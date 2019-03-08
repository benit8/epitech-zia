/*
** EPITECH PROJECT, 2019
** zia
** File description:
** TestModule.cpp
*/

#include "TestModule.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace Modules
{

////////////////////////////////////////////////////////////////////////////////

Test::Test()
: AModule("Test")
{
}

Test::~Test()
{
}

////////////////////////////////////////////////////////////////////////////////

bool Test::onContentGen(HTTP::Request &req, HTTP::Response &res)
{
	res.status(HTTP::Response::Ok);
	res["Content-Type"] = "text/html";
	res.body("<!DOCTYPE html><html><head><meta charset=\"UTF-8\" /><title>Zia | " + req["Host"] + "</title></head><body><h1>Hello World!</h1><h2>from " + req["Host"] + "</h2></body></html>");

	return true;
}

////////////////////////////////////////////////////////////////////////////////

}

////////////////////////////////////////////////////////////////////////////////

extern "C"
{
	IModule *loadModule()
	{
		return new Modules::Test();
	}

	void unloadModule(Modules::Test *mod)
	{
		delete mod;
	}
}