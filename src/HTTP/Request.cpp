/*
** EPITECH PROJECT, 2019
** zia
** File description:
** Request.cpp
*/

#include "Request.hpp"
#include "Logger.hpp"
#include <limits>

////////////////////////////////////////////////////////////////////////////////

namespace HTTP
{

////////////////////////////////////////////////////////////////////////////////

const std::regex Request::urlRegex = std::regex("^(?:(http(?:s)?):\\/\\/)?(?:([\\w\\.-]+)+(?:\\:([\\d]+))?)?(\\/[^\\?]*)(?:\\?(.*))?$");
const std::regex Request::versionRegex = std::regex("^HTTP\\/([\\d]+)\\.([\\d]+)$");

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
: m_port("80")
, m_versionMajor(0)
, m_versionMinor(0)
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
		Logger::error() << "Request::parseRequest(): Invalid HTTP method'" << m_method << "'" << std::endl;
		return false;
	}

	iss >> m_uri;
	std::smatch match;
	if (!std::regex_search(m_uri, match, urlRegex)) {
		Logger::error() << "Request::parseRequest(): Failed to match URI '" << m_uri << "'" << std::endl;
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

	m_protocol = match[1];
	m_host = match[2];
	m_port = match[3].length() == 0 ? "80" : match[3].str();
	m_uri = match[4];
	m_query = match[5];

	std::string versionString;
	std::smatch versionMatch;
	iss >> versionString;
	if (!std::regex_search(versionString, versionMatch, versionRegex)) {
		Logger::error() << "Request::parseRequest(): Failed to match HTTP version" << std::endl;
		return false;
	}
	m_versionMajor = std::stoi(versionMatch[1]);
	m_versionMinor = std::stoi(versionMatch[2]);

	iss.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	parseFields(iss);

	if (m_host.empty()) {
		std::string host = getField("Host");
		std::size_t portSep = host.find(':');
		m_host = host.substr(0, portSep);
		m_port = portSep != std::string::npos ? host.substr(portSep + 1) : "80";
	}

	return true;
}

std::string Request::version()
{
	return "HTTP/" + std::to_string(m_versionMajor) + "." + std::to_string(m_versionMinor);
}

std::string Request::url()
{
	std::string url = m_protocol + "://" + m_host;
	if (m_port != "80" || (m_protocol == "https" && m_port != "443"))
		url += ":" + m_port;
	url += m_uri;
	if (!m_query.empty())
		url += "?" + m_query;
	return url;
}

////////////////////////////////////////////////////////////////////////////////

}