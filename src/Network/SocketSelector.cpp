/*
** EPITECH PROJECT, 2018
** R-Type server
** File description:
** SocketSelector.cpp
*/

#include "Network/SocketSelector.hpp"
#include "Network/SocketImpl.hpp"
#include <algorithm>
#include <iostream>
#include <utility>

////////////////////////////////////////////////////////////////////////////////

namespace Net
{

////////////////////////////////////////////////////////////////////////////////

struct SocketSelector::SocketSelectorImpl
{
	fd_set allSockets;
	fd_set socketsReady;
	int maxSocket;
	int socketCount;
};

////////////////////////////////////////////////////////////////////////////////

SocketSelector::SocketSelector()
: m_impl(new SocketSelectorImpl())
{
	clear();
}

SocketSelector::SocketSelector(const SocketSelector &copy)
: m_impl(new SocketSelectorImpl(*copy.m_impl))
{
}

SocketSelector::~SocketSelector()
{
	std::unique_lock<std::mutex> lock(m_mutex);

	delete m_impl;
}

////////////////////////////////////////////////////////////////////////////////

void SocketSelector::add(Socket &socket)
{
	std::unique_lock<std::mutex> lock(m_mutex);

	Socket::Handle handle = socket.getHandle();
	if (handle != priv::SocketImpl::invalidSocket()) {
#ifdef _WIN32
		if (m_impl->socketCount >= FD_SETSIZE) {
			std::cerr << "The socket can't be added to the selector because the selector is full. This is a limitation of your operating system's FD_SETSIZE setting." << std::endl;
			return;
		}

		if (FD_ISSET(handle, &m_impl->allSockets))
			return;

		m_impl->socketCount++;
#else
		if (handle >= FD_SETSIZE) {
			std::cerr << "The socket can't be added to the selector because its ID is too high. This is a limitation of your operating system's FD_SETSIZE setting." << std::endl;
			return;
		}

		m_impl->maxSocket = std::max(m_impl->maxSocket, handle);
#endif
		FD_SET(handle, &m_impl->allSockets);
	}
}

void SocketSelector::remove(Socket &socket)
{
	std::unique_lock<std::mutex> lock(m_mutex);

	Socket::Handle handle = socket.getHandle();
	if (handle != priv::SocketImpl::invalidSocket()) {
#ifdef _WIN32
		if (!FD_ISSET(handle, &m_impl->allSockets))
			return;

		m_impl->socketCount--;
#else
		if (handle >= FD_SETSIZE)
			return;
#endif
		FD_CLR(handle, &m_impl->allSockets);
		FD_CLR(handle, &m_impl->socketsReady);
	}
}

void SocketSelector::clear()
{
	std::unique_lock<std::mutex> lock(m_mutex);

	FD_ZERO(&m_impl->allSockets);
	FD_ZERO(&m_impl->socketsReady);

	m_impl->maxSocket = 0;
	m_impl->socketCount = 0;
}

bool SocketSelector::wait(unsigned timeout)
{
	std::unique_lock<std::mutex> lock(m_mutex);

	timeval time;
	time.tv_sec  = static_cast<long>(timeout / 1000);
	time.tv_usec = static_cast<long>(timeout * 1000);

	m_impl->socketsReady = m_impl->allSockets;

	int count = select(m_impl->maxSocket + 1, &m_impl->socketsReady, NULL, NULL, timeout != 0 ? &time : NULL);
	return count > 0;
}

bool SocketSelector::isReady(Socket &socket)
{
	std::unique_lock<std::mutex> lock(m_mutex);

	Socket::Handle handle = socket.getHandle();
	if (handle != priv::SocketImpl::invalidSocket()) {
#ifndef _WIN32
		if (handle >= FD_SETSIZE)
			return false;
#endif
		return FD_ISSET(handle, &m_impl->socketsReady) != 0;
	}

	return false;
}

size_t SocketSelector::count()
{
	std::unique_lock<std::mutex> lock(m_mutex);

	return m_impl->socketCount;
}

SocketSelector &SocketSelector::operator=(const SocketSelector &right)
{
	std::unique_lock<std::mutex> lock(m_mutex);

	SocketSelector temp(right);
	std::swap(m_impl, temp.m_impl);
	return *this;
}

////////////////////////////////////////////////////////////////////////////////

}