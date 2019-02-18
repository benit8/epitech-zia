/*
** EPITECH PROJECT, 2019
** zia
** File description:
** IModule.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

class IModule;

////////////////////////////////////////////////////////////////////////////////

#include "HTTP.hpp"

#include <string>

////////////////////////////////////////////////////////////////////////////////

class IModule
{
public:
	virtual ~IModule() {}

	virtual const std::string &getName() = 0;
	virtual bool handle(HTTP::Request &req, HTTP::Response &res, HTTP::ProcessingList &pl) = 0;
};


class AModule : public IModule
{
public:
	AModule(const std::string &name)
	: m_name(name)
	{}

	virtual ~AModule() {}

public:
	const std::string &getName() { return m_name; }

private:
	std::string m_name;
};