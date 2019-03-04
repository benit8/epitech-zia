/*
** EPITECH PROJECT, 2018
** R-Type server
** File description:
** Windows / SocketImpl.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace Net {
	namespace priv {
		class SocketImpl;
	}
}

////////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32_WINDOWS
	#undef _WIN32_WINDOWS
#endif
#ifdef _WIN32_WINNT
	#undef _WIN32_WINNT
#endif
#define _WIN32_WINDOWS 0x0501
#define _WIN32_WINNT   0x0501

////////////////////////////////////////////////////////////////////////////////

#include "Network/Socket.hpp"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>

////////////////////////////////////////////////////////////////////////////////

namespace Net
{

namespace priv
{

////////////////////////////////////////////////////////////////////////////////

class SocketImpl
{
public:

	typedef int AddrLength;

	static sockaddr_in createAddress(uint32_t address, uint16_t port)
	{
		sockaddr_in addr;
		std::memset(&addr, 0, sizeof(addr));
		addr.sin_addr.s_addr = htonl(address);
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);

		return addr;
	}

	static SocketHandle invalidSocket()
	{
		return INVALID_SOCKET;
	}

	static void close(SocketHandle handle)
	{
		closesocket(handle);
	}

	static void setBlocking(SocketHandle handle, bool block)
	{
		u_long blocking = block ? 0 : 1;
		ioctlsocket(sock, FIONBIO, &blocking);
	}

	static Socket::Status getErrorStatus()
	{
		switch (WSAGetLastError()) {
			case WSAEWOULDBLOCK:
			case WSAEALREADY:
				return Socket::NotReady;
			case WSAECONNABORTED:
			case WSAECONNRESET:
			case WSAETIMEDOUT:
			case WSAENETRESET:
			case WSAENOTCONN:
				return Socket::Disconnected;
			case WSAEISCONN:
				return Socket::Done;
			default:
				return Socket::Error;
		}
	}
};

////////////////////////////////////////////////////////////////////////////////

struct SocketInitializer
{
	SocketInitializer()
	{
		WSADATA init;
		WSAStartup(MAKEWORD(2, 2), &init);
	}

	~SocketInitializer()
	{
		WSACleanup();
	}
};

SocketInitializer globalInitializer;

////////////////////////////////////////////////////////////////////////////////

}

}