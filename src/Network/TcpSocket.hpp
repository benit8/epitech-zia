/*
** EPITECH PROJECT, 2018
** R-Type server
** File description:
** TcpSocket.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace Net {
	class TcpSocket;
}

////////////////////////////////////////////////////////////////////////////////

#include "Socket.hpp"
#include "IpAddress.hpp"
#include "Packet.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace Net
{

class TcpSocket : public Socket
{
public:
	TcpSocket();

	uint16_t getLocalPort() const;
	IpAddress getRemoteAddress() const;
	uint16_t getRemotePort() const;

	Status connect(const IpAddress &remoteAddress, uint16_t remotePort, unsigned timeout = 0);
	void disconnect();

	Status send(const void *data, std::size_t size);
	Status send(const void *data, std::size_t size, std::size_t &sent);
	Status send(Packet &packet);
	Status receive(void *data, std::size_t size, std::size_t &received);
	Status receive(Packet &packet);

private:
	friend class TcpListener;

	struct PendingPacket
	{
		PendingPacket();

		uint32_t size;
		std::size_t sizeReceived;
		std::vector<char> data;
	};

	PendingPacket m_pendingPacket;
};

std::ostream &operator<<(std::ostream &stream, TcpSocket &socket);

}