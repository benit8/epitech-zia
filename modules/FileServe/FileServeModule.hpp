/*
** EPITECH PROJECT, 2019
** zia
** File description:
** FileServeModule.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace Modules {
	class FileServe;
}

////////////////////////////////////////////////////////////////////////////////

#include "Module.hpp"

#include <fstream>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

////////////////////////////////////////////////////////////////////////////////

namespace Modules
{

	class EXPORT FileServe : public AModule
	{
	public:
		static const std::map<std::string, std::string> mimeTypes;

	public:
		FileServe(ModuleLoader *ml);
		~FileServe();

		bool onContentGen(json &host, HTTP::Request &req, HTTP::Response &res);

		// Unused
		bool onConnection(json &, std::shared_ptr<Net::TcpSocket>) { return false; }
		bool onReceive(json &, std::shared_ptr<Net::TcpSocket>, std::string &) { return false; }
		bool onParsing(json &, const std::string &, HTTP::Request &) { return false; }
		bool onSend(json &, std::shared_ptr<Net::TcpSocket>, const std::string &) { return false; }
		bool checkModule() { return false; }

	private:
		std::string getMimeType(const fs::path &path);
	};

}