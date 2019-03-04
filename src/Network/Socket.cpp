/*
** EPITECH PROJECT, 2018
** R-Type server
** File description:
** Socket.cpp
*/

#include <iostream>
#include "Network/Socket.hpp"
#include "Network/SocketImpl.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace Net
{

////////////////////////////////////////////////////////////////////////////////

Socket::Socket(Type type)
: m_type(type)
, m_handle(priv::SocketImpl::invalidSocket())
, m_isBlocking(true)
{
}

Socket::~Socket()
{
	close();
}

////////////////////////////////////////////////////////////////////////////////

void Socket::setBlocking(bool blocking)
{
	if (m_handle != priv::SocketImpl::invalidSocket())
		priv::SocketImpl::setBlocking(m_handle, blocking);

	m_isBlocking = blocking;
}

bool Socket::isBlocking() const
{
	return m_isBlocking;
}

Socket::Handle Socket::getHandle() const
{
	return m_handle;
}

////////////////////////////////////////////////////////////////////////////////

void Socket::create()
{
	if (m_handle == priv::SocketImpl::invalidSocket()) {
		Socket::Handle handle = socket(PF_INET, m_type == Type::Tcp ? SOCK_STREAM : SOCK_DGRAM, 0);
		if (handle == priv::SocketImpl::invalidSocket()) {
			std::cerr << "Failed to create socket" << std::endl;
			return;
		}

		create(handle);
	}
}

void Socket::create(Socket::Handle handle)
{
	int yes = 1;
	if (m_handle == priv::SocketImpl::invalidSocket()) {
		m_handle = handle;
		setBlocking(m_isBlocking);

		if (m_type == Type::Tcp) {
			if (setsockopt(m_handle, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char *>(&yes), sizeof(yes)) == -1) {
				std::cerr << "Failed to set socket option \"TCP_NODELAY\"" << std::endl;
			}
		}

#ifdef __APPLE__
		if (setsockopt(m_handle, SOL_SOCKET, SO_NOSIGPIPE, reinterpret_cast<char *>(&yes), sizeof(yes)) == -1) {
			std::cerr << "Failed to set socket option \"SO_NOSIGPIPE\"" << std::endl;
		}
#endif
	}
	else {
		if (setsockopt(m_handle, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char *>(&yes), sizeof(yes)) == -1) {
			std::cerr << "Failed to enable broadcast on UDP socket" << std::endl;
		}
	}
}

void Socket::close()
{
	if (m_handle != priv::SocketImpl::invalidSocket()) {
		priv::SocketImpl::close(m_handle);
		m_handle = priv::SocketImpl::invalidSocket();
	}
}

////////////////////////////////////////////////////////////////////////////////

std::ostream &operator<<(std::ostream &stream, const Socket::Status &status)
{
	switch (status) {
		case Socket::Done:
			stream << "Done";
			break;
		case Socket::NotReady:
			stream << "Not ready";
			break;
		case Socket::Partial:
			stream << "Partial data";
			break;
		case Socket::Disconnected:
			stream << "Disconnected";
			break;
		case Socket::Error:
			stream << "Unknown error";
			break;
	}
	return stream;
}

}