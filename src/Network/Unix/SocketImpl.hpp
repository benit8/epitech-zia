/*
** EPITECH PROJECT, 2018
** R-Type server
** File description:
** Unix / SocketImpl.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace Net {
	namespace priv {
		class SocketImpl;
	}
}

////////////////////////////////////////////////////////////////////////////////

#include "Network/Socket.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////

namespace Net
{

namespace priv
{

////////////////////////////////////////////////////////////////////////////////

class SocketImpl
{
public:
	typedef socklen_t AddrLength;

	static sockaddr_in createAddress(uint32_t address, uint16_t port)
	{
		sockaddr_in addr;
		std::memset(&addr, 0, sizeof(addr));
		addr.sin_addr.s_addr = htonl(address);
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
#ifdef __APPLE__
		addr.sin_len = sizeof(addr);
#endif
		return addr;
	}

	static Socket::Handle invalidSocket()
	{
		return -1;
	}

	static void close(Socket::Handle handle)
	{
		::close(handle);
	}

	static void setBlocking(Socket::Handle handle, bool block)
	{
		int status = fcntl(handle, F_GETFL);
		if (block)
			status &= ~O_NONBLOCK;
		else
			status |= O_NONBLOCK;

		if (fcntl(handle, F_SETFL, status) == -1)
			std::cerr << "Failed to set file status flags: " << errno << std::endl;
	}

	static Socket::Status getErrorStatus()
	{
		if ((errno = EAGAIN) || (errno == EINPROGRESS))
			return Socket::NotReady;

		switch (errno) {
			case EWOULDBLOCK:
				return Socket::NotReady;
			case ECONNABORTED:
			case ECONNRESET:
			case ETIMEDOUT:
			case ENETRESET:
			case ENOTCONN:
			case EPIPE:
				return Socket::Disconnected;
			default:
				return Socket::Error;
		}
	}
};

////////////////////////////////////////////////////////////////////////////////

}

}