/*
** EPITECH PROJECT, 2019
** zia
** File description:
** Zia.cpp
*/

#include "Zia.hpp"

using namespace std::placeholders;

////////////////////////////////////////////////////////////////////////////////

std::list<Zia *> Zia::s_instances;

////////////////////////////////////////////////////////////////////////////////

Zia::Zia(const std::string &configFilename)
: m_configFilename(configFilename)
, m_running(false)
, m_workers(5)
{
	if (s_instances.size() < 1) {
		std::signal(SIGINT, Zia::dispatchSignal);
		std::signal(SIGPIPE, Zia::dispatchSignal);
	}

	s_instances.push_back(this);
}

Zia::~Zia()
{
	s_instances.remove(this);
}

////////////////////////////////////////////////////////////////////////////////

void Zia::loadConfig()
{
	std::ifstream ifs(m_configFilename);
	if (!ifs.is_open())
		Logger::error() << "Failed to open configuration file '" << m_configFilename << "'" << std::endl;
	else
		ifs >> m_config;
}

void Zia::loadModules()
{
	// Load actual modules
	m_moduleLoader.setModulesPath(m_config["Modules"]["Path"].get<std::string>());
	auto moduleNames = m_config["Modules"]["List"].get<std::vector<std::string>>();
	for (auto it = moduleNames.begin(); it != moduleNames.end(); ++it) {
		try {
			m_moduleLoader.loadModule(*it);
			Logger::info() << "Module '" << *it << "' loaded" << std::endl;
		}
		catch (std::runtime_error &e) {
			Logger::error() << "Failed to load module '" << *it << "': " << e.what() << std::endl;
		}
	}

	// Setup hooks
	m_hooks = m_config["Hooks"].get<std::map<std::string, std::string>>();
}

int Zia::run()
{
	for (auto &host : m_config["Hosts"]) {
		for (auto &address : host["Address"])
			createListener(host, address);
	}

	m_running = true;
	while (m_running) {
		if (m_selector.wait())
			handleNetworkEvent();
	}

	m_workers.stop();

	for (auto &vars : m_aliveSockets) {
		m_selector.remove(vars->socket);
		vars->socket.disconnect();
	}

	for (auto &host : m_hosts) {
		host.first->close();
		m_selector.remove(*host.first);
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

bool Zia::createListener(json &host, const std::string &address)
{
	Net::IpAddress addr;
	std::uint16_t port;
	if (!parseHostAddress(address, addr, port)) {
		Logger::error() << "Invalid address '" << address << "' in host " << host["Name"] << std::endl;
		return false;
	}

	auto listener = std::make_shared<Net::TcpListener>();
	if (listener->listen(port, addr) != Net::Socket::Done) {
		Logger::error() << "Failed to listen to " << address << std::endl;
		return false;
	}

	m_selector.add(*listener);
	m_hosts[listener].push_back(host);

	Logger::info() << "Listening to " << address << " for " << host["Name"] << std::endl;
	return true;
}

bool Zia::parseHostAddress(const std::string &hostString, Net::IpAddress &address, std::uint16_t &port)
{
	std::regex reg("^((?:[\\d]+\\.[\\d]+\\.[\\d]+\\.[\\d]+)|\\*):([\\d]+)$");
	std::smatch match;

	if (!std::regex_search(hostString, match, reg))
		return false;

	if (match[1] == "*")
		address = Net::IpAddress::Any;
	else {
		address = match[1];
		if (!address.isValid())
			return false;
	}

	port = std::stoi(match[2]);
	if (port == 0) {
		Logger::warning() << "Warning: port 0 used" << std::endl;
	}

	return true;
}

void Zia::handleNetworkEvent()
{
	for (auto &host : m_hosts) {
		if (m_selector.isReady(*host.first)) {
			handleListenerEvent(host.first, host.second[0]);
		}
	}

	for (auto it = m_aliveSockets.begin(); it != m_aliveSockets.end(); ++it) {
		auto vars = *it;
		if (vars->lock)
			continue;

		if (m_selector.isReady(vars->socket)) {
			vars->lock.lock();
			m_workers.push([&](int, std::shared_ptr<SocketVars> v) {
				if (handleSocketEvent(v->socket, v->host))
					v->lock.unlock();
				else {
					m_selector.remove(v->socket);
					v->socket.disconnect();
					v->tbd = true;
				}
			}, vars);
		}
	}

	{
		std::unique_lock<std::mutex> lock(m_socketMutex);
		m_aliveSockets.remove_if([](std::shared_ptr<SocketVars> vars) { return vars->tbd; });
	}
}

void Zia::handleListenerEvent(std::shared_ptr<Net::TcpListener> listener, json &host)
{
	std::shared_ptr<SocketVars> vars = std::make_shared<SocketVars>(host);
	Net::Socket::Status status = listener->accept(vars->socket);
	if (status != Net::Socket::Done) {
		Logger::warning() << "-- Failed to accept new client: (" << status << ")" << std::endl;
		return;
	}
	else {
		Logger::info() << "++ Connection from " << vars->socket << std::endl;
	}

	if (!onConnection(host, vars->socket)) {
		// If we block the socket (by disconnecting it)
		if (vars->socket.getRemoteAddress() == Net::IpAddress::None)
			return;
	}

	m_selector.add(vars->socket);
	{
		std::unique_lock<std::mutex> lock(m_socketMutex);
		m_aliveSockets.push_back(vars);
	}
}

bool Zia::handleSocketEvent(Net::TcpSocket &socket, json &host)
{
	std::string buffer;
	if (!onReceive(host, socket, buffer))
		return false;

	////////////////////////////////////////////////////////////////////////

	HTTP::Request req;
	req["__remoteAddress"] = socket.getRemoteAddress().toString();
	req["__remotePort"] = std::to_string(socket.getRemotePort());
	if (!onParsing(host, buffer, req))
		Logger::error() << "Zia::handleSocketEvent(): Failed to parse client request" << std::endl;

	////////////////////////////////////////////////////////////////////////

	HTTP::Response res;
	if (!onContentGen(host, req, res))
		Logger::error() << "Zia::handleSocketEvent(): Failed to generate content" << std::endl;

	////////////////////////////////////////////////////////////////////////

	std::string rawRes = res.prepare();
	onSend(host, socket, rawRes);

	////////////////////////////////////////////////////////////////////////

	if (req["Connection"] == "close")
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////////

void Zia::dispatchSignal(int signum)
{
	std::for_each(s_instances.begin(), s_instances.end(), std::bind2nd(std::mem_fun(&Zia::handleSignal), signum));
}

void Zia::handleSignal(int signum)
{
	std::cout << std::endl;
	Logger::warning() << "Caught signal " << signum << ": " << strsignal(signum) << std::endl;
	m_running = false;
}