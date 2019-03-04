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

bool Test::onContentGen(HTTP::Request &/*req*/, HTTP::Response &res)
{
	res.status(HTTP::Response::Ok);
	res["Content-Type"] = "text/plain";
	res.body("Test");

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