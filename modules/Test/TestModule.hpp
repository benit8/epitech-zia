/*
** EPITECH PROJECT, 2019
** zia
** File description:
** TestModule.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace Modules {
	class Test;
}

////////////////////////////////////////////////////////////////////////////////

#include "Export.hpp"
#include "Module.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace Modules
{

	class EXPORT Test : public AModule
	{
	public:
		Test();
		~Test();

		bool onContentGen(HTTP::Request &req, HTTP::Response &res);

		// Unused
		bool onConnection(Net::TcpSocket &) { return false; }
		bool onReceive(Net::TcpSocket &, HTTP::Request &) { return false; }
		bool onSend(Net::TcpSocket &, HTTP::Response &) { return false; }
		bool checkModule() { return false; }
	};

}
