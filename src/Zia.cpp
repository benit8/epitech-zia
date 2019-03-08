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
		std::cerr << "Failed to open configuration file '" << m_configFilename << "'" << std::endl;
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
			std::cout << "Module '" << *it << "' loaded" << std::endl;
		}
		catch (std::runtime_error &e) {
			std::cerr << "Failed to load module '" << *it << "': " << e.what() << std::endl;
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

	for (auto &host : m_hosts) {
		host.first->close();
		m_selector.remove(*host.first);
		delete host.first;
	}

	for (auto &sock : m_aliveSockets) {
		std::cout << "deleting socket" << std::endl;
		m_selector.remove(*sock);
		sock->disconnect();
		delete sock;
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

bool Zia::createListener(const json &host, const std::string &address)
{
	Net::IpAddress addr;
	std::uint16_t port;
	if (!parseHostAddress(address, addr, port)) {
		std::cerr << "Invalid address '" << address << "' in host " << host["Name"] << std::endl;
		return false;
	}

	Net::TcpListener *listener = new Net::TcpListener;
	if (listener->listen(port, addr) != Net::Socket::Done) {
		std::cerr << "Failed to listen to " << address << std::endl;
		delete listener;
		return false;
	}

	m_selector.add(*listener);
	m_hosts[listener].push_back(host);

	std::cout << "Listening to " << address << " for " << host["Name"] << std::endl;
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
		std::cout << "Warning: port 0 used" << std::endl;
	}

	return true;
}

void Zia::handleNetworkEvent()
{
	for (auto &host : m_hosts) {
		if (m_selector.isReady(*host.first))
			handleListenerEvent(host.first);
	}
	for (Net::TcpSocket *socket : m_aliveSockets) {
		if (m_selector.isReady(*socket))
			m_workers.push([&](int, Net::TcpSocket *sock) { handleSocketEvent(sock); }, socket);
	}
}

void Zia::handleListenerEvent(Net::TcpListener *listener)
{
	Net::TcpSocket *socket = new Net::TcpSocket;
	Net::Socket::Status status = listener->accept(*socket);
	if (status != Net::Socket::Done) {
		std::cerr << "-- Failed to accept new client: (" << status << ")" << std::endl;
		delete socket;
		return;
	}
	else {
		std::cout << "++ Connection from " << *socket << std::endl;
	}

	m_workers.push([&](int, Net::TcpSocket *sock) { handleSocketEvent(sock, true); }, socket);
}

void Zia::handleSocketEvent(Net::TcpSocket *socket, bool newCon)
{
	if (newCon && !onConnection(socket)) {
		if (socket->getRemoteAddress() == Net::IpAddress::None) {
			delete socket;
			return;
		}
	}

	////////////////////////////////////////////////////////////////////////

	std::string buffer;
	if (!onReceive(socket, buffer)) {
		socket->disconnect();
		delete socket;
		return;
	}

	////////////////////////////////////////////////////////////////////////

	HTTP::Request req;
	HTTP::Response res;
	onParsing(buffer, req);

	////////////////////////////////////////////////////////////////////////

	onContentGen(req, res);

	////////////////////////////////////////////////////////////////////////

	std::string rawRes = res.prepare();
	onSend(socket, rawRes);

	////////////////////////////////////////////////////////////////////////

	if (!newCon) {
		if (req["Connection"] == "keep-alive") {
			m_aliveSockets.push_back(socket);
			m_selector.add(*socket);
		}
		else {
			socket->disconnect();
			delete socket;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

void Zia::dispatchSignal(int signum)
{
	std::for_each(s_instances.begin(), s_instances.end(), std::bind2nd(std::mem_fun(&Zia::handleSignal), signum));
}

void Zia::handleSignal(int signum)
{
	std::cout << std::endl;
	std::cout << "Caught signal " << signum << ": " << strsignal(signum) << std::endl;
	m_running = false;
}