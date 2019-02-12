/*
** EPITECH PROJECT, 2018
** R-Type server
** File description:
** IpAddress.cpp
*/

#include "Network/IpAddress.hpp"
#include "SocketImpl.hpp"
#include "Network/TcpSocket.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <limits>
#include <sstream>

////////////////////////////////////////////////////////////////////////////////

namespace Net
{

////////////////////////////////////////////////////////////////////////////////

const IpAddress IpAddress::None;
const IpAddress IpAddress::Any(0, 0, 0, 0);
const IpAddress IpAddress::LocalHost(127, 0, 0, 1);
const IpAddress IpAddress::Broadcast(255, 255, 255, 255);

////////////////////////////////////////////////////////////////////////////////

IpAddress::IpAddress()
: m_address(0)
, m_valid(false)
{
}

IpAddress::IpAddress(const char *address)
: m_address(0)
, m_valid(false)
{
	resolve(address);
}

IpAddress::IpAddress(const std::string &address)
: m_address(0)
, m_valid(false)
{
	resolve(address);
}

IpAddress::IpAddress(uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3)
: m_address(htonl((byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3))
, m_valid(true)
{
}

IpAddress::IpAddress(uint32_t address)
: m_address(htonl(address))
, m_valid(true)
{
}

////////////////////////////////////////////////////////////////////////////////

std::string IpAddress::toString() const
{
	in_addr address;
	address.s_addr = m_address;

	return inet_ntoa(address);
}

uint32_t IpAddress::toInteger() const
{
	return ntohl(m_address);
}

////////////////////////////////////////////////////////////////////////////////

IpAddress IpAddress::getLocalAddress()
{
	IpAddress localAddress;

	Socket::Handle sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == priv::SocketImpl::invalidSocket())
		return localAddress;

	sockaddr_in address = priv::SocketImpl::createAddress(ntohl(INADDR_LOOPBACK), 9);
	if (connect(sock, reinterpret_cast<sockaddr *>(&address), sizeof(address)) == -1) {
		priv::SocketImpl::close(sock);
		return localAddress;
	}

	priv::SocketImpl::AddrLength size = sizeof(address);
	if (getsockname(sock, reinterpret_cast<sockaddr *>(&address), &size) == -1) {
		priv::SocketImpl::close(sock);
		return localAddress;
	}

	priv::SocketImpl::close(sock);

	localAddress = IpAddress(ntohl(address.sin_addr.s_addr));
	return localAddress;
}

IpAddress IpAddress::getPublicAddress(unsigned timeout)
{
	TcpSocket socket;
	if (socket.connect("ipinfo.io", 80, timeout) == Socket::Done) {
		const char *req = "GET /ip HTTP/1.1\r\nHost: ipinfo.io\r\nUser-Agent: libcppnet/0.1.0\r\nConnection: close\r\n\r\n";

		if (socket.send(req, strlen(req)) == Socket::Done) {
			std::string receivedStr;
			std::size_t size = 0;
			char buffer[1024];
			while (socket.receive(buffer, sizeof(buffer), size) == Socket::Done)
				receivedStr.append(buffer, buffer + size);

			std::istringstream iss(receivedStr);
			iss.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::string line;
			while (std::getline(iss, line) && (line.length() > 1)) {}
			std::getline(iss, line);
			return IpAddress(line);
		}

		socket.disconnect();
	}

	return IpAddress();
}

////////////////////////////////////////////////////////////////////////////////

void IpAddress::resolve(const std::string &address)
{
	m_address = 0;
	m_valid = false;

	if (address == "255.255.255.255") {
		m_address = INADDR_BROADCAST;
		m_valid = true;
	}
	else if (address == "0.0.0.0") {
		m_address = INADDR_ANY;
		m_valid = true;
	}
	else {
		uint32_t ip = inet_addr(address.c_str());
		if (ip != INADDR_NONE) {
			m_address = ip;
			m_valid = true;
		}
		else {
			addrinfo hints;
			std::memset(&hints, 0, sizeof(hints));
			hints.ai_family = AF_INET;
			addrinfo *result = NULL;
			if (getaddrinfo(address.c_str(), NULL, &hints, &result) == 0) {
				if (result) {
					ip = reinterpret_cast<sockaddr_in *>(result->ai_addr)->sin_addr.s_addr;
					freeaddrinfo(result);
					m_address = ip;
					m_valid = true;
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

bool operator==(const IpAddress &left, const IpAddress &right)
{
	return !(left < right) && !(right < left);
}

bool operator!=(const IpAddress &left, const IpAddress &right)
{
	return !(left == right);
}

bool operator<(const IpAddress &left, const IpAddress &right)
{
	return std::make_pair(left.m_valid, left.m_address) < std::make_pair(right.m_valid, right.m_address);
}

bool operator>(const IpAddress &left, const IpAddress &right)
{
	return right < left;
}

bool operator<=(const IpAddress &left, const IpAddress &right)
{
	return !(right < left);
}

bool operator>=(const IpAddress &left, const IpAddress &right)
{
	return !(left < right);
}

std::istream &operator>>(std::istream &stream, IpAddress &address)
{
	std::string str;
	stream >> str;
	address = IpAddress(str);

	return stream;
}

std::ostream &operator<<(std::ostream &stream, const IpAddress &address)
{
	return stream << address.toString();
}

////////////////////////////////////////////////////////////////////////////////

}