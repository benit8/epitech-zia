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
#include "Logger.hpp"
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

using namespace nlohmann;

////////////////////////////////////////////////////////////////////////////////

class Zia
{
private:
	struct SocketVars
	{
		Net::TcpSocket socket;
		json host;
		std::mutex mutex;
		std::unique_lock<std::mutex> lock;
		bool tbd;

		SocketVars(json host_)
		: socket()
		, host(host_)
		, mutex()
		, lock(mutex, std::defer_lock)
		, tbd(false)
		{}
	};

public:
	Zia(const std::string &configFilename);
	~Zia();

public:
	void loadConfig();
	void loadModules();
	int run();

private:
	bool createListener(json &host, const std::string &address);
	bool parseHostAddress(const std::string &hostString, Net::IpAddress &address, std::uint16_t &port);
	void handleNetworkEvent();
	void handleListenerEvent(std::shared_ptr<Net::TcpListener> listener, json &host);
	bool handleSocketEvent(Net::TcpSocket &socket, json &host);

	bool onConnection(json &host, Net::TcpSocket &socket) {
		auto hook = m_hooks.find("Connection");
		if (hook == m_hooks.end())
			return false;
		IModule *mod = m_moduleLoader.getModule(hook->second);
		return mod == nullptr ? false : mod->onConnection(host, socket);
	}
	bool onReceive(json &host, Net::TcpSocket &socket, std::string &buffer) {
		auto hook = m_hooks.find("Receive");
		if (hook == m_hooks.end())
			return false;
		IModule *mod = m_moduleLoader.getModule(hook->second);
		return mod == nullptr ? false : mod->onReceive(host, socket, buffer);
	}
	bool onParsing(json &host, std::string &buffer, HTTP::Request &req) {
		auto hook = m_hooks.find("Parsing");
		if (hook == m_hooks.end())
			return false;
		IModule *mod = m_moduleLoader.getModule(hook->second);
		return mod == nullptr ? false : mod->onParsing(host, buffer, req);
	}
	bool onContentGen(json &host, HTTP::Request &req, HTTP::Response &res) {
		auto hook = m_hooks.find("ContentGen");
		if (hook == m_hooks.end())
			return false;
		IModule *mod = m_moduleLoader.getModule(hook->second);
		return mod == nullptr ? false : mod->onContentGen(host, req, res);
	}
	bool onSend(json &host, Net::TcpSocket &socket, std::string &buffer) {
		auto hook = m_hooks.find("Send");
		if (hook == m_hooks.end())
			return false;
		IModule *mod = m_moduleLoader.getModule(hook->second);
		return mod == nullptr ? false : mod->onSend(host, socket, buffer);
	}

	static void dispatchSignal(int signum);
	void handleSignal(int signum);

private:
	std::string m_configFilename;
	json m_config;

	ModuleLoader m_moduleLoader;

	bool m_running;
	Net::SocketSelector m_selector;
	std::map<std::shared_ptr<Net::TcpListener>, json> m_hosts;
	std::list<std::shared_ptr<SocketVars>> m_aliveSockets;
	std::map<std::string, std::string> m_hooks;
	ThreadPool m_workers;
	std::mutex m_socketMutex;

	static std::list<Zia *> s_instances;
};