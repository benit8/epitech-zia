/*
** EPITECH PROJECT, 2019
** zia
** File description: SSL Module header
** SSLModule.hpp
*/

#include "SSLModule.hpp"

////////////////////////////////////////////////////////////////////////////////

SSLmod::SSLmod(ModuleLoader *ml)
  : AModule(ml, "SSL")
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
}

////////////////////////////////////////////////////////////////////////////////

bool SSLmod::onReceive(json& host, Net::TcpSocket &socket, std::string &rawReq)
{
  SSL	*ssl;

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
    this->m_isEnabled = false;
  if (SSL_CTX_use_PrivateKey_file(m_ctx, keyFile.c_str(),
				  SSL_FILETYPE_PEM) < 1)
    this->m_isEnabled = false;
  //SSL Struct
  ssl = SSL_new(this->m_ctx);
  if (!ssl) {
    this->m_isEnabled = false;
    ERR_print_errors_fp(stdout);
  } 
  SSL_set_fd(ssl, client);
  ERR_print_errors_fp(stdout);
  if (m_isEnabled == false)
    return false;
  // Reached SSL struct construction lets accept the guy in

  if (SSL_accept(ssl) <= 0)
    ERR_print_errors_fp(stdout);
  std::cout << "the guy is in port ->" << m_port << std::endl;

  // show certificate and get peer certs
  X509	*cert;
  std::string	line;

  cert = SSL_get_peer_certificate(ssl);
  if (cert) {
    std::cout << "Server certificates:" << std::endl;
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
	std::cout << "Subject: " << line << std::endl;
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
	std::cout << "Issuer: " << line << std::endl;
        X509_free(cert);
  }
  const std::size_t readSize = 1024;
  std::size_t len = 0;
  for (;;) {
    if (socket.getRemoteAddress() == Net::IpAddress::None ||
	socket.getRemoteAddress() == Net::IpAddress::Any)
      return false;
    char	buff[readSize] = {0};
    std::size_t recv = SSL_read(ssl, buff, readSize);
    
    if (recv <= 0) {
      Logger::error() << "SSLmod::onReceive() : data recepetion fail. (SSL_read returned <= 0) " << socket << std::endl;
      return false;
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

bool	SSLmod::onSend(json& /*host*/, Net::TcpSocket & /*sock*/, const std::string &/*buffer*/)
{
  std::cout << "in onSend method" << std::endl;
  return true;
}

bool	SSLmod::checkModule()
{
  // std::cout << FBLU("Started testing of SSL module") << std::endl;
  // std::cout << FYEL("Testing CTX declaration");
  // m_ctx = SSL_CTX_new(SSLv23_server_method());
  // if (!m_ctx)
  //   std::cout << FRED("\t\tFAIL") << std::endl;
  // else
  //   std::cout << FGRN("\t\t\tOK") << std::endl;
  // std::cout << FYEL("Testing certificate load");
  // if (SSL_CTX_use_certificate_file(m_ctx, "./certs/cert.pem",
  // 				   SSL_FILETYPE_PEM) < 1)
  //   std::cout << FRED("\t\tFAIL") << std::endl;
  // else
  //   std::cout << FGRN("\t\tOK") << std::endl;
  // std::cout << FYEL("Testing private key load");
  // if (SSL_CTX_use_PrivateKey_file(m_ctx, "./certs/key.pem",
  // 				  SSL_FILETYPE_PEM) < 1)
  //   std::cout << FRED("\t\tFAIL") << std::endl;
  // else
  //   std::cout << FGRN("\t\tOK") << std::endl;
  // std::cout << FYEL("Testing SSL structure creation");
  // m_ssl = SSL_new(m_ctx);
  // if (!m_ssl)
  //   std::cout << FRED("\t\tFAIL") << std::endl;
  // else
  //   std::cout << FGRN("\t\tOK") << std::endl;
  // std::cout << FBLU("Ended testing of SSL module.") << std::endl;
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
