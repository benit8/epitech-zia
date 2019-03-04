/*
** EPITECH PROJECT, 2018
** R-Type server
** File description:
** SocketImpl.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
	#include "Windows/SocketImpl.hpp"
#else
	#include "Unix/SocketImpl.hpp"
#endif