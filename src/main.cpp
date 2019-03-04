/*
** EPITECH PROJECT, 2019
** zia
** File description:
** main.cpp
*/

#include "HTTP.hpp"
#include "Zia.hpp"

int main(int argc, char **argv)
{
	Zia zia(argc == 1 ? "./zia.json" : argv[1]);
	zia.loadConfig();
	zia.loadModules();
	return zia.run();


	// HTTP::Request req;
	// req.parseRequest("GET /products#pred?id=1&page=2 HTTP/1.1\r\nHost: https://www.example.com\r\nUser-Agent: curl/7.60.0\r\nAccept: */*\r\n");
	// return 0;
}