/*
** EPITECH PROJECT, 2019
** zia
** File description: SSL Module header
** SSLModule.hpp
*/

#include "SSLModule.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace Modules
{

////////////////////////////////////////////////////////////////////////////////

  SSL::SSL()
    : AModule("SSL"), m_name("SSL_module")
  {
    SSL_library_init();
    SSL_load_error_strings();
  }

  SSL::~SSL()
  {
  }

  ////////////////////////////////////////////////////////////////////////////////

  bool SSL::onReceive(Net::TcpSocket &/*sock*/, HTTP::Request &/*req*/)
  {
    return true;
  }

  bool	SSL::onSend(Net::TcpSocket &/*sock*/, HTTP::Response &/*resp*/)
  {
    return true;
  }


////////////////////////////////////////////////////////////////////////////////

}

////////////////////////////////////////////////////////////////////////////////

extern "C"
{
	IModule *loadModule()
	{
		return new Modules::SSL();
	}

	void unloadModule(Modules::SSL *mod)
	{
	delete mod;
	}
}
