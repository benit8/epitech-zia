//
// EPITECH PROJECT, 2019
// Zia server
// File Description:
// Main.cpp
//

#include "Server.hpp"

int main(int ac, char **av)
{
	try {
		if (ac == 2) {
			Server server();
		} else {
			std::cout << "Wrong number of arguments" << std::endl;
			std::cout << "Format: ./zia_server <port>" << std::endl;
			return (84);
		}
	}
	catch (std::exception &e) {
			std::cerr << e.what() << std::endl;
	}
	return 0;
}
