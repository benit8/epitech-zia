/*
** EPITECH PROJECT, 2018
** R-Type server
** File description:
** TcpSocket.cpp
*/

#include "Network/TcpSocket.hpp"
#include "SocketImpl.hpp"
#include <algorithm>
#include <cstring>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////

namespace
{
#ifdef __linux__
	const int flags = MSG_NOSIGNAL;
#else
	const int flags = 0;
#endif
}

namespace Net
{

////////////////////////////////////////////////////////////////////////////////

TcpSocket::TcpSocket()
: Socket(Tcp)
{
}

////////////////////////////////////////////////////////////////////////////////

uint16_t TcpSocket::getLocalPort() const
{
	if (getHandle() != priv::SocketImpl::invalidSocket()) {
		sockaddr_in address;
		priv::SocketImpl::AddrLength size = sizeof(address);
		if (getsockname(getHandle(), reinterpret_cast<sockaddr *>(&address), &size) != -1)
			return ntohs(address.sin_port);
	}

	return 0;
}

IpAddress TcpSocket::getRemoteAddress() const
{
	if (getHandle() != priv::SocketImpl::invalidSocket()) {
		sockaddr_in address;
		priv::SocketImpl::AddrLength size = sizeof(address);
		if (getpeername(getHandle(), reinterpret_cast<sockaddr *>(&address), &size) != -1)
			return IpAddress(ntohl(address.sin_addr.s_addr));
	}

	return IpAddress::None;
}

uint16_t TcpSocket::getRemotePort() const
{
	if (getHandle() != priv::SocketImpl::invalidSocket()) {
		sockaddr_in address;
		priv::SocketImpl::AddrLength size = sizeof(address);
		if (getpeername(getHandle(), reinterpret_cast<sockaddr *>(&address), &size) != -1)
			return ntohs(address.sin_port);
	}

	return 0;
}

Socket::Status TcpSocket::connect(const IpAddress& remoteAddress, uint16_t remotePort, unsigned timeout)
{
	disconnect();
	create();

	sockaddr_in address = priv::SocketImpl::createAddress(remoteAddress.toInteger(), remotePort);

	if (timeout <= 0) {
		if (::connect(getHandle(), reinterpret_cast<sockaddr *>(&address), sizeof(address)) == -1)
			return priv::SocketImpl::getErrorStatus();
		return Done;
	}
	else {
		bool blocking = isBlocking();

		if (blocking)
			setBlocking(false);

		if (::connect(getHandle(), reinterpret_cast<sockaddr *>(&address), sizeof(address)) >= 0) {
			setBlocking(blocking);
			return Done;
		}

		Status status = priv::SocketImpl::getErrorStatus();

		if (!blocking)
			return status;

		if (status == Socket::NotReady) {
			fd_set selector;
			FD_ZERO(&selector);
			FD_SET(getHandle(), &selector);

			timeval time;
			time.tv_sec  = static_cast<long>(timeout / 1000);
			time.tv_usec = static_cast<long>(timeout * 1000);

			if (select(static_cast<int>(getHandle() + 1), NULL, &selector, NULL, &time) > 0) {
				if (getRemoteAddress() != IpAddress::None)
					status = Done;
				else
					status = priv::SocketImpl::getErrorStatus();
			}
			else {
				status = priv::SocketImpl::getErrorStatus();
			}
		}

		setBlocking(true);

		return status;
	}
}

void TcpSocket::disconnect()
{
	close();

	m_pendingPacket = PendingPacket();
}

Socket::Status TcpSocket::send(const void *data, std::size_t size)
{
	if (!isBlocking())
		std::cerr << "Warning: Partial sends might not be handled properly." << std::endl;

	std::size_t sent;

	return send(data, size, sent);
}

Socket::Status TcpSocket::send(const void *data, std::size_t size, std::size_t &sent)
{
	if (!data || (size == 0))
	{
		std::cerr << "Cannot send data over the network (no data to send)" << std::endl;
		return Error;
	}

	int result = 0;
	for (sent = 0; sent < size; sent += result) {
		result = ::send(getHandle(), static_cast<const char *>(data) + sent, static_cast<int>(size - sent), flags);
		if (result < 0) {
			Status status = priv::SocketImpl::getErrorStatus();
			if ((status == NotReady) && sent)
				return Partial;
			return status;
		}
	}

	return Done;
}

Socket::Status TcpSocket::receive(void *data, std::size_t size, std::size_t &received)
{
	received = 0;

	if (!data) {
		std::cerr << "Cannot receive data from the network (the destination buffer is invalid)" << std::endl;
		return Error;
	}

	int sizeReceived = recv(getHandle(), static_cast<char *>(data), static_cast<int>(size), flags);
	if (sizeReceived > 0) {
		received = static_cast<std::size_t>(sizeReceived);
		return Done;
	}
	else if (sizeReceived == 0) {
		return Socket::Disconnected;
	}
	else {
		return priv::SocketImpl::getErrorStatus();
	}
}

Socket::Status TcpSocket::send(Packet &packet)
{
	std::size_t size = 0;
	const void *data = packet.onSend(size);
	uint32_t packetSize = htonl(static_cast<uint32_t>(size));
	std::vector<char> blockToSend(sizeof(packetSize) + size);

	std::memcpy(&blockToSend[0], &packetSize, sizeof(packetSize));
	if (size > 0)
		std::memcpy(&blockToSend[0] + sizeof(packetSize), data, size);

	std::size_t sent;
	Status status = send(&blockToSend[0] + packet.m_sendPos, blockToSend.size() - packet.m_sendPos, sent);

	if (status == Partial)
		packet.m_sendPos += sent;
	else if (status == Done)
		packet.m_sendPos = 0;

	return status;
}

Socket::Status TcpSocket::receive(Packet &packet)
{
	packet.clear();

	uint32_t packetSize = 0;
	std::size_t received = 0;
	if (m_pendingPacket.sizeReceived < sizeof(m_pendingPacket.size)) {
		while (m_pendingPacket.sizeReceived < sizeof(m_pendingPacket.size)) {
			char *data = reinterpret_cast<char *>(&m_pendingPacket.size) + m_pendingPacket.sizeReceived;
			Status status = receive(data, sizeof(m_pendingPacket.size) - m_pendingPacket.sizeReceived, received);
			m_pendingPacket.sizeReceived += received;

			if (status != Done)
				return status;
		}

		packetSize = ntohl(m_pendingPacket.size);
	}
	else {
		packetSize = ntohl(m_pendingPacket.size);
	}

	char buffer[1024];
	while (m_pendingPacket.data.size() < packetSize) {
		std::size_t sizeToGet = std::min(static_cast<std::size_t>(packetSize - m_pendingPacket.data.size()), sizeof(buffer));
		Status status = receive(buffer, sizeToGet, received);
		if (status != Done)
			return status;

		if (received > 0) {
			m_pendingPacket.data.resize(m_pendingPacket.data.size() + received);
			char *begin = &m_pendingPacket.data[0] + m_pendingPacket.data.size() - received;
			std::memcpy(begin, buffer, received);
		}
	}

	if (!m_pendingPacket.data.empty())
		packet.onReceive(&m_pendingPacket.data[0], m_pendingPacket.data.size());
	m_pendingPacket = PendingPacket();

	return Done;
}

////////////////////////////////////////////////////////////////////////////////

TcpSocket::PendingPacket::PendingPacket()
: size(0)
, sizeReceived(0)
, data()
{
}

////////////////////////////////////////////////////////////////////////////////

std::ostream &operator<<(std::ostream &stream, TcpSocket &socket)
{
	stream << socket.getRemoteAddress() << ":" << socket.getRemotePort();
	return stream;
}

}