/*
** EPITECH PROJECT, 2019
** zia
** File description: SSL Module header
** SSLModule.hpp
*/

#include "SSLModule.hpp"
#include <iostream>

namespace Modules
{
  SSL::SSL()
    :AModule("SSL")
  {
    std::cout << "Module SSL loaded" << std::endl;
  }

  SSL::~SSL()
  {
  }

  bool SSL::handle(HTTP::Request &/*req*/, HTTP::Response &res, HTTP::ProcessingList &/*pl*/)
  {
    res.status(HTTP::Response::NotImplemented);
    res["Content-Type"] = "text/plain";
    res.body("SSL");
    
    return true;
  }
}

extern "C"
{
  IModule *loadModule()
  {
    return new Modules::SSL();
  }

  void	unloadModule(Modules::SSL *mod)
  {
    delete mod;
  }
}
