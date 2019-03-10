/*
** EPITECH PROJECT, 2019
** zia
** File description:
** HTTPModule.cpp
*/

#include "HTTPModule.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace Modules
{

////////////////////////////////////////////////////////////////////////////////

HTTPMod::HTTPMod(ModuleLoader *ml)
: AModule(ml, "HTTP")
{
}

HTTPMod::~HTTPMod()
{
}

////////////////////////////////////////////////////////////////////////////////

bool HTTPMod::onReceive(const json &host, std::shared_ptr<Net::TcpSocket> socket, std::string &rawReq)
{
	rawReq.clear();

	// Do we use SSL ?
	if (host.count("SSL") > 0 && host["SSL"].is_object()) {
		IModule *ssl = m_ml->getModule("SSL");
		if (ssl == nullptr) {
			std::cerr << "HTTPMod::onReceive(): Could not get SSL module" << std::endl;
			return false;
		}
		return ssl->onReceive(host, socket, rawReq);
	}

	// Normal read
	const std::size_t readSize = 1024;
	std::size_t length = 0;
	for (;;) {
		char buffer[readSize] = {0};
		std::size_t received = 0;
		Net::Socket::Status s = socket->receive(buffer, readSize, received);
		if (s != Net::Socket::Done) {
			std::cerr << "Data reception failed (" << s << ") " << *socket << std::endl;
			return false;
		}

		rawReq.append(buffer, received);
		length += received;
		if (received < readSize)
			break;
	}

	std::cout << ">> Received " << rawReq.length() << " bytes " << *socket << std::endl << rawReq;

	return true;
}

bool HTTPMod::onParsing(const json &/*host*/, const std::string &buffer, HTTP::Request &req)
{
	return req.parseRequest(buffer);
}

bool HTTPMod::onContentGen(const json& /*host*/, HTTP::Request &req, HTTP::Response &res)
{
	res.status(HTTP::Response::Ok);
	res["Content-Type"] = "text/html";
	res.body("<!DOCTYPE html><html><head><meta charset=\"UTF-8\" /><title>Zia | " + req["Host"] + "</title></head><body><h1>Hello World!</h1><h2>from " + req["Host"] + "</h2></body></html>");

	return true;
}

bool HTTPMod::onSend(const json &host, std::shared_ptr<Net::TcpSocket> socket, const std::string &buffer)
{
	// Do we use SSL ?
	if (host.count("SSL") > 0 && host["SSL"].is_object()) {
		IModule *ssl = m_ml->getModule("SSL");
		if (ssl == nullptr) {
			std::cerr << "HTTPMod::onSend(): Could not get SSL module" << std::endl;
			return false;
		}
		return ssl->onSend(host, socket, buffer);
	}

	// Normal send
	std::cout << "<< Sending " << buffer.length() << " bytes " << *socket << std::endl << buffer << std::endl << std::endl;
	return socket->send(buffer.c_str(), buffer.length()) != Net::Socket::Done;
}

////////////////////////////////////////////////////////////////////////////////

}

////////////////////////////////////////////////////////////////////////////////

extern "C"
{
	IModule *loadModule(ModuleLoader *ml)
	{
		return new Modules::HTTPMod(ml);
	}

	void unloadModule(Modules::HTTPMod *mod)
	{
		delete mod;
	}
}