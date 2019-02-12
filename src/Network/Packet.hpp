/*
** EPITECH PROJECT, 2018
** R-Type server
** File description:
** Packet.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace Net {
	class Packet;
}

////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

namespace Net
{

class Packet
{
	typedef bool (Packet::*BoolType)(std::size_t);

public:
	Packet();
	virtual ~Packet() = default;

	template <typename... Args>
	Packet(Args... args)
	: Packet() {
		(*this << ... << args);
	}

	void append(const void *data, std::size_t size);
	void clear();
	const void *getData() const;
	std::size_t getDataSize() const;
	bool endOfPacket() const;

public:
	operator BoolType() const;

	Packet &operator>>(bool &data);
	Packet &operator>>(int8_t &data);
	Packet &operator>>(uint8_t &data);
	Packet &operator>>(int16_t &data);
	Packet &operator>>(uint16_t &data);
	Packet &operator>>(int32_t &data);
	Packet &operator>>(uint32_t &data);
	Packet &operator>>(int64_t &data);
	Packet &operator>>(uint64_t &data);
	Packet &operator>>(float &data);
	Packet &operator>>(double &data);
	Packet &operator>>(char *data);
	Packet &operator>>(std::string &data);

	Packet &operator<<(bool data);
	Packet &operator<<(int8_t data);
	Packet &operator<<(uint8_t data);
	Packet &operator<<(int16_t data);
	Packet &operator<<(uint16_t data);
	Packet &operator<<(int32_t data);
	Packet &operator<<(uint32_t data);
	Packet &operator<<(int64_t data);
	Packet &operator<<(uint64_t data);
	Packet &operator<<(float data);
	Packet &operator<<(double data);
	Packet &operator<<(const char *data);
	Packet &operator<<(const std::string &data);

protected:
	virtual const void *onSend(std::size_t &size);
	virtual void onReceive(const void *data, std::size_t size);

	friend class TcpSocket;
	friend class UdpSocket;

private:
	bool checkSize(std::size_t size);

	bool operator==(const Packet &right) const;
	bool operator!=(const Packet &right) const;

	std::vector<char> m_data;
	std::size_t m_readPos;
	std::size_t m_sendPos;
	bool m_isValid;
};

}