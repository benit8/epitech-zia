/*
** EPITECH PROJECT, 2019
** Zia
** FIle description: SSL Module header
** SSLModule.hpp
*/

#pragma once

namespace Modules {
  class SSL;
}

#include "../../common/Export.hpp"
#include "../../common/Module.hpp"

namespace Modules
{
  class EXPORT SSL : public AModule
  {
  public:
    SSL();
    ~SSL();

    bool handle(HTTP::Request &req, HTTP::Response &res, HTTP::ProcessingList &pl);
    
  private:
    std::string m_name;
  };
}
