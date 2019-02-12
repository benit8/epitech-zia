/*
** EPITECH PROJECT, 2018
** R-Type server
** File description:
** NonCopyable.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

class NonCopyable
{
protected:
	NonCopyable() = default;
	~NonCopyable() = default;

private:
	NonCopyable(const NonCopyable &);
	NonCopyable &operator=(const NonCopyable &);
};