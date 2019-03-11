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
  SSL_load_error_strings();
}

SSLmod::~SSLmod()
{
}

////////////////////////////////////////////////////////////////////////////////

bool SSLmod::onReceive(json& host, std::shared_ptr<Net::TcpSocket> sock, std::string &/*buffer*/)
{
  /*
  ** Un exemple d'utilisation du `json &host`:
  ** ---------------------------------------------------------------------------
  ** std::string certFile = host["SSL"]["Cert"].get<std::string>();
  ** std::string keyFile = host["SSL"]["Key"].get<std::string>();
  **  ...
  ** SSL_CTX_use_certificate_file(m_ctx, certFile.c_str(), SSL_FILETYPE_PEM)
  ** SSL_CTX_use_PrivateKey_file(m_ctx, keyFile.c_str(), SSL_FILETYPE_PEM)
  ** ---------------------------------------------------------------------------
  */

  m_socket = sock->getHandle();
  m_port = sock->getRemotePort();
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

bool	SSLmod::onSend(json& /*host*/, std::shared_ptr<Net::TcpSocket> /*sock*/, const std::string &/*buffer*/)
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
  IModule *loadModule(ModuleLoader *ml)
  {
    return new SSLmod(ml);
  }

  void unloadModule(SSLmod *mod)
  {
    delete mod;
  }
}
