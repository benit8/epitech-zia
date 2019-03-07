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

HTTPMod::HTTPMod()
: AModule("HTTP")
{
}

HTTPMod::~HTTPMod()
{
}

////////////////////////////////////////////////////////////////////////////////

bool HTTPMod::onReceive(Net::TcpSocket *socket, std::string &rawReq)
{
	const std::size_t readSize = 128;

	std::size_t length = 0;
	for (;;) {
		char buffer[readSize] = {0};
		std::size_t received = 0;
		Net::Socket::Status s = socket->receive(buffer, readSize, received);
		if (s != Net::Socket::Done) {
			std::cerr << "Data reception failed (" << s << ")" << std::endl;
			return false;
		}

		rawReq.append(buffer, received);
		length += received;
		if (received < readSize)
			break;
	}

	std::cout << ">> Received " << rawReq.length() << " bytes" << std::endl << rawReq;

	return true;
}

bool HTTPMod::onParsing(const std::string &buffer, HTTP::Request &req)
{
	return req.parseRequest(buffer);
}

bool HTTPMod::onSend(Net::TcpSocket *socket, const std::string &buffer)
{
	std::cout << "<< Sending " << buffer.length() << " bytes" << std::endl << buffer << std::endl << std::endl;

	return socket->send(buffer.c_str(), buffer.length()) != Net::Socket::Done;
}

////////////////////////////////////////////////////////////////////////////////

}

////////////////////////////////////////////////////////////////////////////////

extern "C"
{
	IModule *loadModule()
	{
		return new Modules::HTTPMod();
	}

	void unloadModule(Modules::HTTPMod *mod)
	{
		delete mod;
	}
}