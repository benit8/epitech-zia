/*
** EPITECH PROJECT, 2019
** zia
** File description:
** Response.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace HTTP {
	class Response;
}

////////////////////////////////////////////////////////////////////////////////

#include "FieldContainer.hpp"

#include <map>

////////////////////////////////////////////////////////////////////////////////

namespace HTTP
{

////////////////////////////////////////////////////////////////////////////////

class Response : public FieldContainer
{
public:
	enum Status
	{
		Continue                     = 100,
		SwitchingProtocols           = 101,

		Ok                           = 200,
		Created                      = 201,
		Accepted                     = 202,
		NonAuthoritativeInformation  = 203,
		NoContent                    = 204,
		ResetContent                 = 205,
		PartialContent               = 206,

		MultipleChoices              = 300,
		MovedPermanently             = 301,
		MovedTemporarily             = 302,
		NotModified                  = 304,
		UseProxy                     = 305,
		TemporaryRedirect            = 307,

		BadRequest                   = 400,
		Unauthorized                 = 401,
		PaymentRequired              = 402,
		Forbidden                    = 403,
		NotFound                     = 404,
		MethodNotAllowed             = 405,
		NotAcceptable                = 406,
		ProxyAuthenticationRequired  = 407,
		RequestTimeOut               = 408,
		Conflict                     = 409,
		Gone                         = 410,
		LengthRequired               = 411,
		PreconditionFailed           = 412,
		RequestEntityTooLarge        = 413,
		RequestURITooLarge           = 414,
		UnsupportedMediaType         = 415,
		RequestedRangeNotSatisfiable = 416,
		ExpectationFailed            = 417,

		InternalServerError          = 500,
		NotImplemented               = 501,
		BadGateway                   = 502,
		ServiceUnavailable           = 503,
		GatewayTimeout               = 504,
		HTTPVersionNotSupported      = 505
	};

	static std::map<Status, std::string> statusStrings;

public:
	Response();
	~Response();

	std::string prepare();

public:
	void status(Status status) { m_status = status; }
	Status status() { return m_status; }
	void body(const std::string &body) { m_body = body; }
	const std::string &body() { return m_body; }

	const std::string &data() { return m_data; }
	std::size_t length() const { return m_dataLength; }

private:
	std::string getDateString();

private:
	Status m_status;
	std::string m_body;

	std::string m_data;
	std::size_t m_dataLength;
};

////////////////////////////////////////////////////////////////////////////////

}