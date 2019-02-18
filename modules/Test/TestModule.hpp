/*
** EPITECH PROJECT, 2019
** zia
** File description:
** TestModule.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace Modules {
	class Test;
}

////////////////////////////////////////////////////////////////////////////////

#include "../../common/Export.hpp"
#include "../../common/Module.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace Modules
{

	class EXPORT Test : public AModule
	{
	public:
		Test();
		~Test();

		bool handle(const HTTP::Request &req, HTTP::Response &res);

	private:
		std::string m_name;
	};

}