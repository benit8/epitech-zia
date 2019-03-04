/*
** EPITECH PROJECT, 2018
** R-Type server
** File description:
** TcpListener.cpp
*/

#include "Network/TcpListener.hpp"
#include "Network/SocketImpl.hpp"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////

namespace Net
{

////////////////////////////////////////////////////////////////////////////////

TcpListener::TcpListener()
: Socket(Tcp)
{
}

////////////////////////////////////////////////////////////////////////////////

uint16_t TcpListener::getLocalPort() const
{
	if (getHandle() != priv::SocketImpl::invalidSocket()) {
		sockaddr_in address;
		priv::SocketImpl::AddrLength size = sizeof(address);
		if (getsockname(getHandle(), reinterpret_cast<sockaddr *>(&address), &size) != -1)
			return ntohs(address.sin_port);
	}

	return 0;
}

Socket::Status TcpListener::listen(uint16_t port, const IpAddress &address)
{
	close();
	create();

	if ((address == IpAddress::None) || (address == IpAddress::Broadcast))
		return Error;

	sockaddr_in addr = priv::SocketImpl::createAddress(address.toInteger(), port);
	if (bind(getHandle(), reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1) {
		std::cerr << "Failed to bind listener socket to port " << port << std::endl;
		return Error;
	}

	if (::listen(getHandle(), SOMAXCONN) == -1) {
		std::cerr << "Failed to listen to port " << port << std::endl;
		return Error;
	}

	return Done;
}

void TcpListener::close()
{
	Socket::close();
}

Socket::Status TcpListener::accept(TcpSocket &socket)
{
	if (getHandle() == priv::SocketImpl::invalidSocket()) {
		std::cerr << "Failed to accept a new connection, the socket is not listening" << std::endl;
		return Error;
	}

	sockaddr_in address;
	priv::SocketImpl::AddrLength length = sizeof(address);
	Socket::Handle remote = ::accept(getHandle(), reinterpret_cast<sockaddr *>(&address), &length);
	if (remote == priv::SocketImpl::invalidSocket())
		return priv::SocketImpl::getErrorStatus();

	socket.close();
	socket.create(remote);

	return Done;
}

////////////////////////////////////////////////////////////////////////////////

}