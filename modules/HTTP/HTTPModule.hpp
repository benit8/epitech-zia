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

#include "Module.hpp"

#include <fstream>

////////////////////////////////////////////////////////////////////////////////

namespace Modules
{

	class EXPORT HTTPMod : public AModule
	{
	public:
		HTTPMod(ModuleLoader *ml);
		~HTTPMod();

		bool onReceive(json &host, Net::TcpSocket &, std::string &) override;
		bool onParsing(json &host, const std::string &buffer, HTTP::Request &req) override;
		bool onContentGen(json &host, HTTP::Request &req, HTTP::Response &res) override;
		bool onSend(json &host, Net::TcpSocket &, const std::string &) override;
	};

}