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

#include "Module.hpp"
#include "./Colors.h"

////////////////////////////////////////////////////////////////////////////////

class EXPORT SSLmod : public AModule
{
public:
	SSLmod(ModuleLoader *ml);
	~SSLmod();

	bool	onReceive(json &host, Net::TcpSocket &socket, std::string &buffer) override;
	bool	onSend(json &host, Net::TcpSocket &socket, const std::string &buffer) override;
	bool	checkModule() override;

private:
	SSL_CTX		*m_ctx;
	SSL		*m_ssl;
	int		m_socket;
	uint16_t	m_port;
	std::string	m_address;
	bool		m_isEnabled;
};

////////////////////////////////////////////////////////////////////////////////
