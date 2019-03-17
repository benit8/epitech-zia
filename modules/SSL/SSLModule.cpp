/*
** EPITECH PROJECT, 2019
** zia
** File description: SSL Module header
** SSLModule.hpp
*/

#include "SSLModule.hpp"

////////////////////////////////////////////////////////////////////////////////

SSLmod::SSLmod(ModuleLoader *ml)
  : AModule(ml)
{
  // Load openssl modules
  OPENSSL_init_ssl(0, NULL);
  OpenSSL_add_ssl_algorithms();
  SSL_load_error_strings();

  // CTX must be generated before hand
  m_ctx = SSL_CTX_new(SSLv23_server_method());

  m_isEnabled = true;
}

SSLmod::~SSLmod()
{
  SSL_free(ssl);
  SSL_CTX_free(m_ctx);
  EVP_cleanup();
}

////////////////////////////////////////////////////////////////////////////////

bool SSLmod::onReceive(json& host, Net::TcpSocket &socket, std::string &rawReq)
{
  // check if client is still alive
  if (socket.getRemoteAddress() == Net::IpAddress::None ||
      socket.getRemoteAddress() == Net::IpAddress::Any)
    return false;

  std::string certFile = host["SSL"]["Cert"].get<std::string>();
  std::string keyFile = host["SSL"]["Key"].get<std::string>();

  int client = socket.getHandle();
  m_port = socket.getRemotePort();

  // cert and key
  if (SSL_CTX_use_certificate_file(m_ctx, certFile.c_str(),
				   SSL_FILETYPE_PEM) < 1)
    return false;
  if (SSL_CTX_use_PrivateKey_file(m_ctx, keyFile.c_str(),
				  SSL_FILETYPE_PEM) < 1)
    return false;
  //SSL Struct
  ssl = SSL_new(this->m_ctx);
  if (!ssl)
    return false;
  if (SSL_set_fd(ssl, client) <= 0)
    return false;
  ERR_print_errors_fp(stdout);
  if (m_isEnabled == false)
    return false;
  // Reached SSL struct construction lets accept the guy in

  int	iResult;
  bool	no_error = true;

  do
    {
      iResult = SSL_accept(ssl);
      switch(SSL_get_error(ssl, iResult))
	{
	case SSL_ERROR_NONE:
	  break;
	case SSL_ERROR_ZERO_RETURN:
	case SSL_ERROR_WANT_READ:
	  break;
	case SSL_ERROR_WANT_WRITE:
	  break;
	case SSL_ERROR_WANT_CONNECT:
	case SSL_ERROR_WANT_ACCEPT:
	case SSL_ERROR_WANT_X509_LOOKUP:
	case SSL_ERROR_SYSCALL:
	case SSL_ERROR_SSL:
	default:
	  no_error = false;
	  break;
	}
    }
  while (iResult < 0 && no_error);
  Logger::info() << "SSLmod::onReceive(): SSL_accept success with " << socket << std::endl;

  // Lets read what he has to say
  const std::size_t readSize = 1024;
  std::size_t len = 0;
  for (;;) {
    if (socket.getRemoteAddress() == Net::IpAddress::None ||
  	socket.getRemoteAddress() == Net::IpAddress::Any)
      return false;
    char	buff[readSize + 1] = {0};
    std::size_t recv = SSL_read(ssl, buff, readSize);

    if (SSL_get_error(ssl, recv) == SSL_ERROR_WANT_READ)
      break;
    if (recv <= 0) {
      Logger::error() << "SSLmod::onReceive() : data recepetion fail. (SSL_read returned <= 0) " << socket << std::endl;
      //      return false;
    }
    buff[recv] = 0;
    rawReq.append(buff, recv);
    len += recv;
    if (recv < readSize)
      break;
  }

  Logger::info() << ">> (SSL) Received " << rawReq.length() << " bytes " << socket << std::endl;
  Logger::debug() << rawReq;

  return true;
}

bool	SSLmod::onSend(json& /*host*/, Net::TcpSocket &socket, const std::string &toSend)
{
  int	iResult;
  bool	no_error = true;

  do {
    iResult = SSL_write(ssl, toSend.c_str(), toSend.size());
    switch (SSL_get_error(ssl, iResult)) {
    case SSL_ERROR_NONE:
      break;
    case SSL_ERROR_ZERO_RETURN:
    case SSL_ERROR_WANT_READ:
    case SSL_ERROR_WANT_WRITE:
      break;
    case SSL_ERROR_WANT_CONNECT:
    case SSL_ERROR_WANT_ACCEPT:
    case SSL_ERROR_WANT_X509_LOOKUP:
    case SSL_ERROR_SYSCALL:
    case SSL_ERROR_SSL:
    default:
      no_error = false;
      break;
    }
  }
  while (iResult > 0 && no_error);
  Logger::info() << "SSLmod::onSend(): sent " << toSend.size() << " bytes "
		 << socket << std::endl;
  Logger::debug() << toSend << std::endl;

  return true;
}

////////////////////////////////////////////////////////////////////////////////

extern "C"
{
  IModule *loadModule(ModuleLoader *ml)
  {
    return new SSLmod(ml);
  }

  void unloadModule(SSLmod *mod)
  {
    delete mod;
  }
}
