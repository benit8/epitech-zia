/*
** EPITECH PROJECT, 2019
** Zia
** FIle description: SSL Module header
** SSLModule.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

class SSLmod;

////////////////////////////////////////////////////////////////////////////////

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/opensslv.h>

#include "../../common/Export.hpp"
#include "../../common/Module.hpp"
#include "./Colors.h"

////////////////////////////////////////////////////////////////////////////////

class EXPORT SSLmod : public AModule
{
public:
	SSLmod();
	~SSLmod();

	bool	onReceive(Net::TcpSocket &, HTTP::Request &);
	bool	onSend(Net::TcpSocket &, HTTP::Response &);
	bool	checkModule();

	// Unused
	bool	onConnection(Net::TcpSocket &) { return false; }
	bool	onContentGen(HTTP::Request &, HTTP::Response &) { return false; }

private:
	SSL_CTX		*m_ctx;
	SSL		*m_ssl;
	int		m_socket;
	uint16_t	m_port;
	std::string	m_address;
	bool		m_isEnabled;
};

////////////////////////////////////////////////////////////////////////////////
