/*
** EPITECH PROJECT, 2019
** zia
** File description: SSL Module header
** SSLModule.hpp
*/

#include "SSLModule.hpp"

////////////////////////////////////////////////////////////////////////////////

SSLmod::SSLmod()
  : AModule("SSL")
{
  // Load openssl modules
  OPENSSL_init_ssl(0, NULL);
  SSL_load_error_strings();
}

SSLmod::~SSLmod()
{
  std::cout << "SSLmod destroyed" << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

bool SSLmod::onReceive(Net::TcpSocket &/*sock*/, HTTP::Request &/*req*/)
{
  //  m_socket = sock.getHandle();
  //  m_port = sock.getRemotePort();
  // Init CTX 
  m_ctx = SSL_CTX_new(SSLv23_server_method());
  if (!m_ctx)
    return false;
  // Set certificate
  if (SSL_CTX_use_certificate_file(m_ctx, "./certs/cert.pem",
				   SSL_FILETYPE_PEM) < 1)
    this->m_isEnabled = false;
  // Set private key
  if (SSL_CTX_use_PrivateKey_file(m_ctx, "./certs/key.pem",
				  SSL_FILETYPE_PEM) < 1)
    this->m_isEnabled = false;
  // Creating SSL Structure
  m_ssl = SSL_new(m_ctx);
  if (!m_ssl)
    this->m_isEnabled = false;
  if (m_isEnabled == false)
    return false;
  std::cout << "Reached construction of SSL structure" << std::endl;
  return true;
}

bool	SSLmod::onSend(Net::TcpSocket &/*sock*/, HTTP::Response &/*resp*/)
{
  return true;
}

bool	SSLmod::checkModule()
{
  std::cout << FBLU("Started testing of SSL module") << std::endl;
  std::cout << FYEL("Testing CTX declaration");
  m_ctx = SSL_CTX_new(SSLv23_server_method());
  if (!m_ctx)
    std::cout << FRED("\t\tFAIL") << std::endl;
  else
    std::cout << FGRN("\t\t\tOK") << std::endl;
  std::cout << FYEL("Testing certificate load");
  if (SSL_CTX_use_certificate_file(m_ctx, "./certs/cert.pem",
				   SSL_FILETYPE_PEM) < 1)
    std::cout << FRED("\t\tFAIL") << std::endl;
  else
    std::cout << FGRN("\t\tOK") << std::endl;
  std::cout << FYEL("Testing private key load");
  if (SSL_CTX_use_PrivateKey_file(m_ctx, "./certs/key.pem",
				  SSL_FILETYPE_PEM) < 1)
    std::cout << FRED("\t\tFAIL") << std::endl;
  else
    std::cout << FGRN("\t\tOK") << std::endl;
  std::cout << FYEL("Testing SSL structure creation");
  m_ssl = SSL_new(m_ctx);
  if (!m_ssl)
    std::cout << FRED("\t\tFAIL") << std::endl;
  else
    std::cout << FGRN("\t\tOK") << std::endl;
  std::cout << FBLU("Ended testing of SSL module.") << std::endl;
  return true;
}

////////////////////////////////////////////////////////////////////////////////

extern "C"
{
  IModule *loadModule()
  {
    return new SSLmod();
  }
  
  void unloadModule(SSLmod *mod)
  {
    delete mod;
  }
}
