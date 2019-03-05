/*
** EPITECH PROJECT, 2019
** zia
** File description:
** Zia.cpp
*/

#include "Zia.hpp"

////////////////////////////////////////////////////////////////////////////////

std::list<Zia *> Zia::s_instances;

////////////////////////////////////////////////////////////////////////////////

Zia::Zia(const std::string &configFilename)
: m_configFilename(configFilename)
, m_running(false)
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
	m_moduleLoader.setModulesPath(m_config["modules"]["path"].get<std::string>());

	auto moduleNames = m_config["modules"]["list"].get<std::vector<std::string>>();
	for (auto it = moduleNames.begin(); it != moduleNames.end(); ++it) {
		try {
			m_moduleLoader.loadModule(*it);
			std::cout << "Module '" << *it << "' loaded" << std::endl;
		}
		catch (std::runtime_error &e) {
			std::cerr << "Failed to load module '" << *it << "': " << e.what() << std::endl;
		}
	}
}

int Zia::run()
{
	// Parse website files ?
	// Get a compilation of all the ports we need to listen to (eg. 80, 443, ...)
	// Boot a TcpListener for each of them, add it to the selector

	for (auto &host : m_config["hosts"]) {
		std::string hostname = host["name"].get<std::string>();
		m_hosts[hostname].clear();
		for (auto &address : host["address"]) {
			Net::IpAddress addr;
			std::uint16_t port;
			if (!parseHostAddress(address, addr, port)) {
				std::cerr << "Invalid address '" << address << "' in host " << hostname << std::endl;
				continue;
			}

			Net::TcpListener *listener = new Net::TcpListener;
			if (listener->listen(port, addr) != Net::Socket::Done) {
				std::cerr << "Failed to listen to " << address << std::endl;
				delete listener;
				continue;
			}

			m_selector.add(*listener);
			m_hosts[hostname].push_back(listener);

			std::cout << "Host " << hostname << ": listening to " << address << std::endl;
		}
	}

	m_running = true;
	while (m_running) {
		if (m_selector.wait())
			handleNetworkEvent();
	}

	for (auto &host : m_hosts) {
		for (auto &listener : host.second) {
			listener->close();
			m_selector.remove(*listener);
			delete listener;
		}
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

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
		for (auto &listener : host.second) {
			if (m_selector.isReady(*listener)) {
				Net::TcpSocket clientSocket;
				Net::Socket::Status status = listener->accept(clientSocket);
				if (status != Net::Socket::Done) {
					std::cerr << "Failed to accept new client: (" << status << ")" << std::endl;
					continue;
				}

				std::cout << "Connection from " << clientSocket << std::endl;

				// triggerHook(Connection);

				std::size_t readSize = 128;
				std::size_t length = 0;
				std::string rawReq;
				for (;;) {
					char buffer[readSize] = {0};
					std::size_t received = 0;
					status = clientSocket.receive(buffer, readSize, received);
					if (status != Net::Socket::Done) {
						std::cerr << "Data reception failed (" << status << ")" << std::endl;
						length = 0;
						break;
					}

					rawReq.append(buffer, received);
					length += received;
					if (received < readSize)
						break;
				}

				if (length == 0)
					continue;

				std::cout << ">> Received " << length << " bytes" << std::endl;
				std::cout << "---------------------------------" << std::endl
				          << rawReq
				          << "---------------------------------" << std::endl << std::endl;

				std::string res = "HTTP/1.1 200 OK\r\nContent-Type: text/plain; charset=utf-8\r\nContent-Length: 12\r\n\r\nHello World!";
				clientSocket.send(res.data(), res.length());

				clientSocket.disconnect();
			}
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