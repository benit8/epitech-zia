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

		bool onContentGen(json &host, HTTP::Request &req, HTTP::Response &res) override;

	private:
		void displayDirectoryContents(const fs::path &p, HTTP::Request &req, HTTP::Response &res);
		std::string getMimeType(const fs::path &path);
	};

}