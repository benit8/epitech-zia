/*
** EPITECH PROJECT, 2019
** zia
** File description:
** Zia.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "json.hpp"
#include "ModuleLoader.hpp"
#include "ThreadPool.hpp"
#include "Network/SocketSelector.hpp"
#include "Network/TcpListener.hpp"
#include "HTTP/Request.hpp"
#include "HTTP/Response.hpp"

#include <algorithm>
#include <csignal>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <string>

using json = nlohmann::json;

////////////////////////////////////////////////////////////////////////////////

class Zia
{
public:
	Zia(const std::string &configFilename);
	~Zia();

public:
	void loadConfig();
	void loadModules();
	int run();

private:
	bool createListener(const json &host, const std::string &address);
	bool parseHostAddress(const std::string &hostString, Net::IpAddress &address, std::uint16_t &port);
	void handleNetworkEvent();
	void handleListenerEvent(Net::TcpListener *listener);
	void handleSocketEvent(std::shared_ptr<Net::TcpSocket> socket);

	bool onConnection(std::shared_ptr<Net::TcpSocket> socket) {
		auto hook = m_hooks.find("Connection");
		if (hook == m_hooks.end())
			return false;
		IModule *mod = m_moduleLoader.getModule(hook->second);
		return mod == nullptr ? false : mod->onConnection(socket);
	}
	bool onReceive(std::shared_ptr<Net::TcpSocket> socket, std::string &buffer) {
		auto hook = m_hooks.find("Receive");
		if (hook == m_hooks.end())
			return false;
		IModule *mod = m_moduleLoader.getModule(hook->second);
		return mod == nullptr ? false : mod->onReceive(socket, buffer);
	}
	bool onParsing(std::string &buffer, HTTP::Request &req) {
		auto hook = m_hooks.find("Parsing");
		if (hook == m_hooks.end())
			return false;
		IModule *mod = m_moduleLoader.getModule(hook->second);
		return mod == nullptr ? false : mod->onParsing(buffer, req);
	}
	bool onContentGen(HTTP::Request &req, HTTP::Response &res) {
		auto hook = m_hooks.find("ContentGen");
		if (hook == m_hooks.end())
			return false;
		IModule *mod = m_moduleLoader.getModule(hook->second);
		return mod == nullptr ? false : mod->onContentGen(req, res);
	}
	bool onSend(std::shared_ptr<Net::TcpSocket> socket, std::string &buffer) {
		auto hook = m_hooks.find("Send");
		if (hook == m_hooks.end())
			return false;
		IModule *mod = m_moduleLoader.getModule(hook->second);
		return mod == nullptr ? false : mod->onSend(socket, buffer);
	}

	static void dispatchSignal(int signum);
	void handleSignal(int signum);

private:
	std::string m_configFilename;
	json m_config;

	ModuleLoader m_moduleLoader;

	bool m_running;
	Net::SocketSelector m_selector;
	std::map<Net::TcpListener *, json> m_hosts;
	std::list<std::shared_ptr<Net::TcpSocket>> m_aliveSockets;
	std::map<std::string, std::string> m_hooks;
	ThreadPool m_workers;

	static std::list<Zia *> s_instances;
};