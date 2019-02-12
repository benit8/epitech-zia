/*
** EPITECH PROJECT, 2018
** R-Type server
** File description:
** UdpSocket.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace Net {
	class UdpSocket;
}

////////////////////////////////////////////////////////////////////////////////

#include "Socket.hpp"
#include "IpAddress.hpp"
#include "Packet.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace Net
{

class UdpSocket : public Socket
{
public:
	enum
	{
		MaxDatagramSize = 65507
	};

public:
	UdpSocket();

	uint16_t getLocalPort() const;

	Status bind(uint16_t port, const IpAddress &address = IpAddress::Any);
	void unbind();

	Status send(const void *data, std::size_t size, const IpAddress &remoteAddress, uint16_t remotePort);
	Status send(Packet &packet, const IpAddress &remoteAddress, uint16_t remotePort);
	Status receive(void *data, std::size_t size, std::size_t &received, IpAddress &remoteAddress, uint16_t &remotePort);
	Status receive(Packet &packet, IpAddress &remoteAddress, uint16_t &remotePort);

private:
	std::vector<char> m_buffer;
};

}