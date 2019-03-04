/*
** EPITECH PROJECT, 2019
** zia
** File description:
** Request.cpp
*/

#include "Request.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace HTTP
{

////////////////////////////////////////////////////////////////////////////////

const std::regex Request::urlRegex = std::regex("^(?:(http(?:s)?):\\/\\/)?([\\w\\.-]+)+(?:\\:([\\d]+))?(\\/.*)(?:\\?(.*))$");

const std::array<std::string, 8> Request::validMethods = {
	"OPTIONS",
	"GET",
	"HEAD",
	"POST",
	"PUT",
	"DELETE",
	"TRACE",
	"CONNECT"
};

const std::array<std::string, 19> Request::validHeaders = {
	"Accept",
	"Accept-Charset",
	"Accept-Encoding",
	"Accept-Language",
	"Authorization",
	"Expect",
	"From",
	"Host",
	"If-Match",
	"If-Modified-Since",
	"If-None-Match",
	"If-Range",
	"If-Unmodified-Since",
	"Max-Forwards",
	"Proxy-Authorization",
	"Range",
	"Referer",
	"TE",
	"User-Agent"
};

////////////////////////////////////////////////////////////////////////////////

Request::Request()
: m_port(80)
{
}

Request::~Request()
{
}

////////////////////////////////////////////////////////////////////////////////

bool Request::parseRequest(const std::string &data)
{
	std::istringstream iss(data);

	iss >> m_method;
	if (std::find(validMethods.begin(), validMethods.end(), m_method) == validMethods.end()) {
		std::cerr << "Invalid HTTP method'" << m_method << "'" << std::endl;
		return false;
	}

	iss >> m_url;
	if (m_url.find("http") == 0) {
		std::smatch match;
		if (!std::regex_search(m_url, match, urlRegex)) {
			std::cerr << "Failed to match absolute URL '" << m_url << "'" << std::endl;
			return false;
		}

		/* `match` content (index)
		** 0: Full match
		** 1: Protocol
		** 2: Full domain name
		** 3: Port
		** 4: URI
		** 5: Query
		*/

		m_host = match[2];
		m_port = match[3].length() == 0 ? 80 : std::stoi(match[3].str());
		m_uri = match[4];
		m_query = match[5];

		return true;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////

}