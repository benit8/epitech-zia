/*
** EPITECH PROJECT, 2018
** R-Type server
** File description:
** IpAddress.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace Net {
	class IpAddress;
}

////////////////////////////////////////////////////////////////////////////////

#include <istream>
#include <ostream>
#include <string>

////////////////////////////////////////////////////////////////////////////////

namespace Net
{

class IpAddress
{
public:
	IpAddress();
	IpAddress(const char *address);
	IpAddress(const std::string &address);
	IpAddress(uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3);
	explicit IpAddress(uint32_t address);

	IpAddress &operator =(const char *address);
	IpAddress &operator =(const std::string &address);
	IpAddress &operator =(uint32_t &address);

	std::string toString() const;
	uint32_t toInteger() const;
	bool isValid() const;

	static IpAddress getLocalAddress();
	static IpAddress getPublicAddress(unsigned timeout = 0);

	static const IpAddress None;
	static const IpAddress Any;
	static const IpAddress LocalHost;
	static const IpAddress Broadcast;

private:
	uint32_t m_address;
	bool m_valid;

	void resolve(const std::string &address);

	friend bool operator<(const IpAddress &left, const IpAddress &right);
};

bool operator==(const IpAddress &left, const IpAddress &right);
bool operator!=(const IpAddress &left, const IpAddress &right);
bool operator<(const IpAddress &left, const IpAddress &right);
bool operator>(const IpAddress &left, const IpAddress &right);
bool operator<=(const IpAddress &left, const IpAddress &right);
bool operator>=(const IpAddress &left, const IpAddress &right);
std::istream &operator>>(std::istream &stream, IpAddress &address);
std::ostream &operator<<(std::ostream &stream, const IpAddress &address);

}