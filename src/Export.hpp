/*
** EPITECH PROJECT, 2019
** zia
** File description:
** Export.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
	#define EXPORT __declspec(dllexport)
#else
	#define EXPORT
#endif