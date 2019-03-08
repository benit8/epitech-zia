/*
** EPITECH PROJECT, 2019
** zia
** File description:
** HTTPModule.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace Modules {
	class HTTPMod;
}

////////////////////////////////////////////////////////////////////////////////

#include "Export.hpp"
#include "Module.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace Modules
{

	class EXPORT HTTPMod : public AModule
	{
	public:
		HTTPMod();
		~HTTPMod();

		bool onReceive(std::shared_ptr<Net::TcpSocket> , std::string &);
		bool onParsing(const std::string &buffer, HTTP::Request &req);
		bool onSend(std::shared_ptr<Net::TcpSocket> , const std::string &);

		// Unused
		bool onConnection(std::shared_ptr<Net::TcpSocket> ) { return false; }
		bool onContentGen(HTTP::Request &, HTTP::Response &) { return false; }
		bool checkModule() { return false; }
	};

}