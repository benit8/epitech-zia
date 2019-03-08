/*
** EPITECH PROJECT, 2018
** R-Type server
** File description:
** Socket.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace Net {
	class Socket;
}

////////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
	#include <basetsd.h>
#endif

#include <iostream>
#include <vector>

#include "NonCopyable.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace Net
{

class Socket : NonCopyable
{
public:
#ifdef _WIN32
	typedef UINT_PTR Handle;
#else
	typedef int Handle;
#endif

	enum Type
	{
		Tcp,
		Udp
	};

	enum Status
	{
		Done,
		NotReady,
		Partial,
		Disconnected,
		Error
	};

	enum
	{
		AnyPort = 0
	};

public:
	virtual ~Socket();

	void setBlocking(bool blocking);
	bool isBlocking() const;

	Handle getHandle() const;

protected:
	Socket(Type type);
	void create();
	void create(Handle handle);
	void close();

private:
	friend class SocketSelector;

	Type m_type;
	Handle m_handle;
	bool m_isBlocking;
};

std::ostream &operator<<(std::ostream &stream, const Socket::Status &status);

}