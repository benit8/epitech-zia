/*
** EPITECH PROJECT, 2019
** zia
** File description:
** HTTP.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include <map>
#include <regex>
#include <string>

////////////////////////////////////////////////////////////////////////////////

namespace HTTP
{
	class FieldContainer
	{
	public:
		virtual ~FieldContainer() = default;

		void clearFields() { m_fields.clear(); }
		void setField(const std::string &key, const std::string &value) { m_fields[key] = value; }
		void unsetField(const std::string &key) { m_fields.erase(key); }
		const std::string &getField(const std::string &key) { return m_fields[key]; }

		std::string operator [](const std::string &key) { return m_fields[key]; }

		void parseFieldData(const std::string &data) {
			std::istringstream iss(data);
			std::string line;
			while (std::getline(iss, line)) {
				const std::regex reg("^([A-Za-z\\-]+)\\s*:\\s*(.*)$");
				std::smatch match;
				if (std::regex_search(line, match, reg) && match.size() == 3)
					m_fields[match[1]] = match[2];
			}
		}

	protected:
		std::map<std::string, std::string> m_fields;
	};

	////////////////////////////////////////////////////////////////////////

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
	public:
		Request();
		~Request();
	};

	////////////////////////////////////////////////////////////////////////

	class Response : public FieldContainer
	{
	public:
		enum Status
		{
			Continue           = 100,
			SwitchingProtocols = 101,

			Ok                          = 200,
			Created                     = 201,
			Accepted                    = 202,
			NonAuthoritativeInformation = 203,
			NoContent                   = 204,
			ResetContent                = 205,
			PartialContent              = 206,

			MultipleChoices   = 300,
			MovedPermanently  = 301,
			MovedTemporarily  = 302,
			NotModified       = 304,
			UseProxy          = 305,
			TemporaryRedirect = 307,

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

			InternalServerError     = 500,
			NotImplemented          = 501,
			BadGateway              = 502,
			ServiceUnavailable      = 503,
			GatewayTimeout          = 504,
			HTTPVersionNotSupported = 505
		};
	public:
		Response();
		~Response();

		void setStatus(Status status) { m_status = status; }
		void setBody(const std::string &body) { m_body = body; }

	private:
		Status m_status;
		std::string m_body;
	};
}