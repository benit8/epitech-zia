/*
** EPITECH PROJECT, 2019
** Zia
** FIle description: SSL Module header
** SSLModule.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace Modules {
	class SSL;
}

////////////////////////////////////////////////////////////////////////////////

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>

#include "../../common/Export.hpp"
#include "../../common/Module.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace Modules
{

////////////////////////////////////////////////////////////////////////////////

class EXPORT SSL : public AModule
{
public:
	SSL();
	~SSL();

	bool	onReceive(Net::TcpSocket &, HTTP::Request &);
	bool	onSend(Net::TcpSocket &, HTTP::Response &);

	// Unused
	bool	onConnection(Net::TcpSocket &) { return false; }
	bool	onContentGen(HTTP::Request &, HTTP::Response &) { return false; }

private:
	std::string	m_name;
	SSL_CTX		*m_ctx;
	SSL		*m_ssl;
	int		m_socket;
	int		m_port;
	std::string	m_address;
	bool		m_isEnabled;
};

////////////////////////////////////////////////////////////////////////////////

}
