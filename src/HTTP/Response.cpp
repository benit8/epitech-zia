/*
** EPITECH PROJECT, 2019
** zia
** File description:
** Response.cpp
*/

#include "Response.hpp"
#include <iomanip>

////////////////////////////////////////////////////////////////////////////////

namespace HTTP
{

////////////////////////////////////////////////////////////////////////////////

std::map<Response::Status, std::string> Response::statusStrings = {
	{Response::Continue, "Continue"},
	{Response::SwitchingProtocols, "SwitchingProtocols"},
	{Response::Ok, "Ok"},
	{Response::Created, "Created"},
	{Response::Accepted, "Accepted"},
	{Response::NonAuthoritativeInformation, "NonAuthoritativeInformation"},
	{Response::NoContent, "NoContent"},
	{Response::ResetContent, "ResetContent"},
	{Response::PartialContent, "PartialContent"},
	{Response::MultipleChoices, "MultipleChoices"},
	{Response::MovedPermanently, "MovedPermanently"},
	{Response::MovedTemporarily, "MovedTemporarily"},
	{Response::NotModified, "NotModified"},
	{Response::UseProxy, "UseProxy"},
	{Response::TemporaryRedirect, "TemporaryRedirect"},
	{Response::BadRequest, "BadRequest"},
	{Response::Unauthorized, "Unauthorized"},
	{Response::PaymentRequired, "PaymentRequired"},
	{Response::Forbidden, "Forbidden"},
	{Response::NotFound, "NotFound"},
	{Response::MethodNotAllowed, "MethodNotAllowed"},
	{Response::NotAcceptable, "NotAcceptable"},
	{Response::ProxyAuthenticationRequired, "ProxyAuthenticationRequired"},
	{Response::RequestTimeOut, "RequestTimeOut"},
	{Response::Conflict, "Conflict"},
	{Response::Gone, "Gone"},
	{Response::LengthRequired, "LengthRequired"},
	{Response::PreconditionFailed, "PreconditionFailed"},
	{Response::RequestEntityTooLarge, "RequestEntityTooLarge"},
	{Response::RequestURITooLarge, "RequestURITooLarge"},
	{Response::UnsupportedMediaType, "UnsupportedMediaType"},
	{Response::RequestedRangeNotSatisfiable, "RequestedRangeNotSatisfiable"},
	{Response::ExpectationFailed, "ExpectationFailed"},
	{Response::InternalServerError, "InternalServerError"},
	{Response::NotImplemented, "NotImplemented"},
	{Response::BadGateway, "BadGateway"},
	{Response::ServiceUnavailable, "ServiceUnavailable"},
	{Response::GatewayTimeout, "GatewayTimeout"},
	{Response::HTTPVersionNotSupported, "HTTPVersionNotSupported"}
};

////////////////////////////////////////////////////////////////////////////////

Response::Response()
: m_status(InternalServerError)
, m_body("")
, m_data("")
{
}

Response::~Response()
{
}

////////////////////////////////////////////////////////////////////////////////

void Response::body(const std::string &body, bool append)
{
	if (append)
		m_body += body;
	else
		m_body = body;
}

void Response::body(std::istream &is, bool append)
{
	size_t pos = is.tellg();
	is.seekg(0, std::ios::end);
	size_t size = is.tellg();
	is.seekg(pos, std::ios::beg);

	if (append)
		size += m_body.length();
	m_body.reserve(size);

	if (!append)
		m_body.clear();

	m_body.append(
		std::istreambuf_iterator<char>(is),
		std::istreambuf_iterator<char>()
	);
}

std::string Response::prepare()
{
	if (!m_data.empty())
		return m_data;

	defaultField("Date", getDateString());
	defaultField("Content-Type", "text/plain");
	defaultField("Content-Length", std::to_string(m_body.length()));

	std::ostringstream oss;
	oss << "HTTP/1.1 " << m_status << " " << statusStrings[m_status] << "\r\n";
	exportFields(oss);
	oss << "\r\n" << m_body;
	m_data = oss.str();

	return m_data;
}

////////////////////////////////////////////////////////////////////////////////

std::string Response::getDateString()
{
	std::time_t t = std::time(nullptr);
	std::tm tm = *std::localtime(&t);

	std::ostringstream oss;
	oss << std::put_time(&tm, "%c");
	return oss.str();
}

////////////////////////////////////////////////////////////////////////////////

}