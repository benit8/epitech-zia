/*
** EPITECH PROJECT, 2018
** R-Type server
** File description:
** UdpSocket.cpp
*/

#include "Network/UdpSocket.hpp"
#include "Network/SocketImpl.hpp"
#include <algorithm>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////

namespace Net
{

////////////////////////////////////////////////////////////////////////////////

UdpSocket::UdpSocket()
: Socket(Udp)
, m_buffer(MaxDatagramSize)
{
}

////////////////////////////////////////////////////////////////////////////////

uint16_t UdpSocket::getLocalPort() const
{
	if (getHandle() != priv::SocketImpl::invalidSocket()) {
		sockaddr_in address;
		priv::SocketImpl::AddrLength size = sizeof(address);
		if (getsockname(getHandle(), reinterpret_cast<sockaddr *>(&address), &size) != -1)
			return ntohs(address.sin_port);
	}

	return 0;
}


Socket::Status UdpSocket::bind(uint16_t port, const IpAddress &address)
{
	close();
	create();

	if ((address == IpAddress::None) || (address == IpAddress::Broadcast))
		return Error;

	sockaddr_in addr = priv::SocketImpl::createAddress(address.toInteger(), port);
	if (::bind(getHandle(), reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1) {
		std::cerr << "Failed to bind socket to port " << port << std::endl;
		return Error;
	}

	return Done;
}

void UdpSocket::unbind()
{
	close();
}


Socket::Status UdpSocket::send(const void *data, std::size_t size, const IpAddress &remoteAddress, uint16_t remotePort)
{
	create();

	if (size > MaxDatagramSize) {
		std::cerr << "Cannot send data over the network (the number of bytes to send is greater than sf::UdpSocket::MaxDatagramSize)" << std::endl;
		return Error;
	}

	sockaddr_in address = priv::SocketImpl::createAddress(remoteAddress.toInteger(), remotePort);
	int sent = sendto(getHandle(), static_cast<const char *>(data), static_cast<int>(size), 0, reinterpret_cast<sockaddr *>(&address), sizeof(address));
	if (sent < 0)
		return priv::SocketImpl::getErrorStatus();

	return Done;
}

Socket::Status UdpSocket::receive(void *data, std::size_t size, std::size_t &received, IpAddress &remoteAddress, uint16_t &remotePort)
{
	received = 0;
	remoteAddress = IpAddress();
	remotePort = 0;

	if (!data) {
		std::cerr << "Cannot receive data from the network (the destination buffer is invalid)" << std::endl;
		return Error;
	}

	sockaddr_in address = priv::SocketImpl::createAddress(INADDR_ANY, 0);
	priv::SocketImpl::AddrLength addressSize = sizeof(address);
	int sizeReceived = recvfrom(getHandle(), static_cast<char *>(data), static_cast<int>(size), 0, reinterpret_cast<sockaddr *>(&address), &addressSize);
	if (sizeReceived < 0)
		return priv::SocketImpl::getErrorStatus();

	received = static_cast<std::size_t>(sizeReceived);
	remoteAddress = IpAddress(ntohl(address.sin_addr.s_addr));
	remotePort = ntohs(address.sin_port);

	return Done;
}

Socket::Status UdpSocket::send(Packet &packet, const IpAddress &remoteAddress, uint16_t remotePort)
{
	std::size_t size = 0;
	const void *data = packet.onSend(size);

	return send(data, size, remoteAddress, remotePort);
}

Socket::Status UdpSocket::receive(Packet &packet, IpAddress &remoteAddress, uint16_t &remotePort)
{
	std::size_t received = 0;
	Status status = receive(&m_buffer[0], m_buffer.size(), received, remoteAddress, remotePort);

	packet.clear();
	if ((status == Done) && (received > 0))
		packet.onReceive(&m_buffer[0], received);

	return status;
}

////////////////////////////////////////////////////////////////////////////////

}