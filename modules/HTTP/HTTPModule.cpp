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

bool HTTPMod::onReceive(json &host, Net::TcpSocket &socket, std::string &rawReq)
{
	rawReq.clear();

	// Do we use SSL ?
	if (host.count("SSL") > 0 && host["SSL"].is_object()) {
		IModule *ssl = m_ml->getModule("SSL");
		if (ssl == nullptr) {
			Logger::error() << "HTTPMod::onReceive(): Could not get SSL module" << std::endl;
			return false;
		}
		return ssl->onReceive(host, socket, rawReq);
	}

	if (socket.getRemoteAddress() == Net::IpAddress::None)
		return false;

	// Normal read
	const std::size_t readSize = 1024;
	std::size_t length = 0;
	for (;;) {
		char buffer[readSize] = {0};
		std::size_t received = 0;
		Net::Socket::Status s = socket.receive(buffer, readSize, received);
		if (s != Net::Socket::Done) {
			Logger::error() << "HTTPMod::onReceive(): Data reception failed (" << s << ") " << socket << std::endl;
			return false;
		}

		rawReq.append(buffer, received);
		length += received;
		if (received < readSize)
			break;
	}

	Logger::info() << ">> Received " << rawReq.length() << " bytes " << socket << std::endl;
	Logger::debug() << rawReq;

	return true;
}

bool HTTPMod::onParsing(json &/*host*/, const std::string &buffer, HTTP::Request &req)
{
	return req.parseRequest(buffer);
}

bool HTTPMod::onContentGen(json &/*host*/, HTTP::Request &/*req*/, HTTP::Response &res)
{
	res.status(HTTP::Response::InternalServerError);

	return true;
}

bool HTTPMod::onSend(json &host, Net::TcpSocket & socket, const std::string &buffer)
{
	// Do we use SSL ?
	if (host.count("SSL") > 0 && host["SSL"].is_object()) {
		IModule *ssl = m_ml->getModule("SSL");
		if (ssl == nullptr) {
			Logger::error() << "HTTPMod::onSend(): Could not get SSL module" << std::endl;
			return false;
		}
		return ssl->onSend(host, socket, buffer);
	}

	// Normal send
	Logger::info() << "<< Sending " << buffer.length() << " bytes " << socket << std::endl;
	Logger::debug() << (buffer.find("text/") != std::string::npos ? buffer : "[Binary data]") << std::endl;
	return socket.send(buffer.c_str(), buffer.length()) != Net::Socket::Done;
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