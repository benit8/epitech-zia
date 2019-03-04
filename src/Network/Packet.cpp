/*
** EPITECH PROJECT, 2018
** R-Type server
** File description:
** Packet.cpp
*/

#include "Network/Packet.hpp"
#include "Network/SocketImpl.hpp"
#include <cstring>

////////////////////////////////////////////////////////////////////////////////

namespace Net
{

////////////////////////////////////////////////////////////////////////////////

Packet::Packet()
: m_readPos(0)
, m_sendPos(0)
, m_isValid(true)
{
}

////////////////////////////////////////////////////////////////////////////////

void Packet::append(const void *data, std::size_t size)
{
	if (data && (size > 0)) {
		std::size_t start = m_data.size();
		m_data.resize(start + size);
		std::memcpy(&m_data[start], data, size);
	}
}

void Packet::clear()
{
	m_data.clear();
	m_readPos = 0;
	m_isValid = true;
}

const void *Packet::getData() const
{
	return !m_data.empty() ? &m_data[0] : NULL;
}

std::size_t Packet::getDataSize() const
{
	return m_data.size();
}

bool Packet::endOfPacket() const
{
	return m_readPos >= m_data.size();
}

////////////////////////////////////////////////////////////////////////////////

Packet::operator BoolType() const
{
	return m_isValid ? &Packet::checkSize : NULL;
}


Packet &Packet::operator>>(bool &data)
{
	uint8_t value;
	if (*this >> value)
		data = (value != 0);

	return *this;
}

Packet &Packet::operator>>(int8_t &data)
{
	if (checkSize(sizeof(data))) {
		data = *reinterpret_cast<const int8_t *>(&m_data[m_readPos]);
		m_readPos += sizeof(data);
	}

	return *this;
}

Packet &Packet::operator>>(uint8_t &data)
{
	if (checkSize(sizeof(data))) {
		data = *reinterpret_cast<const uint8_t *>(&m_data[m_readPos]);
		m_readPos += sizeof(data);
	}

	return *this;
}

Packet &Packet::operator>>(int16_t &data)
{
	if (checkSize(sizeof(data))) {
		data = ntohs(*reinterpret_cast<const int16_t *>(&m_data[m_readPos]));
		m_readPos += sizeof(data);
	}

	return *this;
}

Packet &Packet::operator>>(uint16_t &data)
{
	if (checkSize(sizeof(data))) {
		data = ntohs(*reinterpret_cast<const uint16_t *>(&m_data[m_readPos]));
		m_readPos += sizeof(data);
	}

	return *this;
}

Packet &Packet::operator>>(int32_t &data)
{
	if (checkSize(sizeof(data))) {
		data = ntohl(*reinterpret_cast<const int32_t *>(&m_data[m_readPos]));
		m_readPos += sizeof(data);
	}

	return *this;
}

Packet &Packet::operator>>(uint32_t &data)
{
	if (checkSize(sizeof(data))) {
		data = ntohl(*reinterpret_cast<const uint32_t *>(&m_data[m_readPos]));
		m_readPos += sizeof(data);
	}

	return *this;
}

Packet &Packet::operator>>(int64_t &data)
{
	if (checkSize(sizeof(data))) {
		const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&m_data[m_readPos]);
		data = (static_cast<int64_t>(bytes[0]) << 56) |
		       (static_cast<int64_t>(bytes[1]) << 48) |
		       (static_cast<int64_t>(bytes[2]) << 40) |
		       (static_cast<int64_t>(bytes[3]) << 32) |
		       (static_cast<int64_t>(bytes[4]) << 24) |
		       (static_cast<int64_t>(bytes[5]) << 16) |
		       (static_cast<int64_t>(bytes[6]) <<  8) |
		       (static_cast<int64_t>(bytes[7]));
		m_readPos += sizeof(data);
	}

	return *this;
}

Packet &Packet::operator>>(uint64_t &data)
{
	if (checkSize(sizeof(data))) {
		const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&m_data[m_readPos]);
		data = (static_cast<uint64_t>(bytes[0]) << 56) |
		       (static_cast<uint64_t>(bytes[1]) << 48) |
		       (static_cast<uint64_t>(bytes[2]) << 40) |
		       (static_cast<uint64_t>(bytes[3]) << 32) |
		       (static_cast<uint64_t>(bytes[4]) << 24) |
		       (static_cast<uint64_t>(bytes[5]) << 16) |
		       (static_cast<uint64_t>(bytes[6]) <<  8) |
		       (static_cast<uint64_t>(bytes[7]));
		m_readPos += sizeof(data);
	}

	return *this;
}

Packet &Packet::operator>>(float &data)
{
	if (checkSize(sizeof(data))) {
		data = *reinterpret_cast<const float *>(&m_data[m_readPos]);
		m_readPos += sizeof(data);
	}

	return *this;
}

Packet &Packet::operator>>(double &data)
{
	if (checkSize(sizeof(data))) {
		data = *reinterpret_cast<const double *>(&m_data[m_readPos]);
		m_readPos += sizeof(data);
	}

	return *this;
}

Packet &Packet::operator>>(char *data)
{
	uint32_t length = 0;
	*this >> length;

	if ((length > 0) && checkSize(length)) {
		std::memcpy(data, &m_data[m_readPos], length);
		data[length] = '\0';

		m_readPos += length;
	}

	return *this;
}

Packet &Packet::operator>>(std::string &data)
{
	uint32_t length = 0;
	*this >> length;

	data.clear();
	if ((length > 0) && checkSize(length)) {
		data.assign(&m_data[m_readPos], length);

		m_readPos += length;
	}

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

Packet &Packet::operator<<(bool data)
{
	*this << static_cast<uint8_t>(data);
	return *this;
}

Packet &Packet::operator<<(int8_t data)
{
	append(&data, sizeof(data));
	return *this;
}

Packet &Packet::operator<<(uint8_t data)
{
	append(&data, sizeof(data));
	return *this;
}

Packet &Packet::operator<<(int16_t data)
{
	int16_t toWrite = htons(data);
	append(&toWrite, sizeof(toWrite));
	return *this;
}

Packet &Packet::operator<<(uint16_t data)
{
	uint16_t toWrite = htons(data);
	append(&toWrite, sizeof(toWrite));
	return *this;
}

Packet &Packet::operator<<(int32_t data)
{
	int32_t toWrite = htonl(data);
	append(&toWrite, sizeof(toWrite));
	return *this;
}

Packet &Packet::operator<<(uint32_t data)
{
	uint32_t toWrite = htonl(data);
	append(&toWrite, sizeof(toWrite));
	return *this;
}

Packet &Packet::operator<<(int64_t data)
{
	uint8_t toWrite[] = {
		static_cast<uint8_t>((data >> 56) & 0xFF),
		static_cast<uint8_t>((data >> 48) & 0xFF),
		static_cast<uint8_t>((data >> 40) & 0xFF),
		static_cast<uint8_t>((data >> 32) & 0xFF),
		static_cast<uint8_t>((data >> 24) & 0xFF),
		static_cast<uint8_t>((data >> 16) & 0xFF),
		static_cast<uint8_t>((data >>  8) & 0xFF),
		static_cast<uint8_t>((data      ) & 0xFF)
	};
	append(&toWrite, sizeof(toWrite));
	return *this;
}

Packet &Packet::operator<<(uint64_t data)
{
	uint8_t toWrite[] = {
		static_cast<uint8_t>((data >> 56) & 0xFF),
		static_cast<uint8_t>((data >> 48) & 0xFF),
		static_cast<uint8_t>((data >> 40) & 0xFF),
		static_cast<uint8_t>((data >> 32) & 0xFF),
		static_cast<uint8_t>((data >> 24) & 0xFF),
		static_cast<uint8_t>((data >> 16) & 0xFF),
		static_cast<uint8_t>((data >>  8) & 0xFF),
		static_cast<uint8_t>((data      ) & 0xFF)
	};
	append(&toWrite, sizeof(toWrite));
	return *this;
}

Packet &Packet::operator<<(float data)
{
	append(&data, sizeof(data));
	return *this;
}

Packet &Packet::operator<<(double data)
{
	append(&data, sizeof(data));
	return *this;
}

Packet &Packet::operator<<(const char *data)
{
	uint32_t length = static_cast<uint32_t>(std::strlen(data));
	*this << length;

	append(data, length * sizeof(char));

	return *this;
}

Packet &Packet::operator<<(const std::string& data)
{
	uint32_t length = static_cast<uint32_t>(data.size());
	*this << length;

	if (length > 0)
		append(data.c_str(), length * sizeof(std::string::value_type));

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

bool Packet::checkSize(std::size_t size)
{
	m_isValid = m_isValid && (m_readPos + size <= m_data.size());

	return m_isValid;
}

////////////////////////////////////////////////////////////////////////////////

const void *Packet::onSend(std::size_t &size)
{
	size = getDataSize();
	return getData();
}

void Packet::onReceive(const void *data, std::size_t size)
{
	append(data, size);
}

////////////////////////////////////////////////////////////////////////////////

}