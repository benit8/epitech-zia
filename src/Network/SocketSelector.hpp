/*
** EPITECH PROJECT, 2018
** R-Type server
** File description:
** SocketSelector.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace Net {
	class SocketSelector;
}

////////////////////////////////////////////////////////////////////////////////

#include <mutex>

#include "Socket.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace Net
{

class SocketSelector
{
public:
	SocketSelector();
	SocketSelector(const SocketSelector &copy);
	~SocketSelector();

	void add(Socket &socket);
	void remove(Socket &socket);
	void clear();
	bool wait(unsigned timeout = 0);
	bool isReady(Socket &socket);
	size_t count();

	SocketSelector &operator=(const SocketSelector &right);

private:
	struct SocketSelectorImpl;

	SocketSelectorImpl *m_impl;
	std::mutex m_mutex;
};

}