/*
** EPITECH PROJECT, 2018
** R-Type server
** File description:
** TcpListener.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace Net {
	class TcpListener;
}

////////////////////////////////////////////////////////////////////////////////

#include "Socket.hpp"
#include "IpAddress.hpp"
#include "TcpSocket.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace Net
{

class TcpListener : public Socket
{
public:
	TcpListener();

	uint16_t getLocalPort() const;

	Status listen(uint16_t port, const IpAddress &address = IpAddress::Any);
	void close();
	Status accept(TcpSocket &socket);
};

}