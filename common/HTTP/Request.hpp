/*
** EPITECH PROJECT, 2019
** zia
** File description:
** Request.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace HTTP {
	class Request;
}

////////////////////////////////////////////////////////////////////////////////

#include "FieldContainer.hpp"

#include <array>
#include <iostream>
#include <regex>

////////////////////////////////////////////////////////////////////////////////

namespace HTTP
{

////////////////////////////////////////////////////////////////////////////////

class Request : public FieldContainer
{
public:
	enum Method
	{
		OPTIONS,
		GET,
		HEAD,
		POST,
		PUT,
		DELETE,
		TRACE,
		CONNECT
	};

	static const std::regex urlRegex;
	static const std::regex versionRegex;
	static const std::array<std::string, 8> validMethods;
	static const std::array<std::string, 19> validHeaders;

public:
	Request();
	~Request();

	bool parseRequest(const std::string &data);

	const std::string &method() const { return m_method; }
	const std::string &host() const { return m_host; }
	std::uint16_t port() const { return m_port; }


private:
	std::string m_method;
	std::string m_host;
	std::uint16_t m_port;
	std::string m_url;
	std::string m_uri;
	std::string m_query;
	int m_versionMajor;
	int m_versionMinor;
};

////////////////////////////////////////////////////////////////////////////////

}