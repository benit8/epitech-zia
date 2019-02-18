/*
** EPITECH PROJECT, 2019
** zia
** File description:
** ProcessingList.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace HTTP {
	class ProcessingList;
}

////////////////////////////////////////////////////////////////////////////////

#include "Request.hpp"
#include "Response.hpp"

#include <functional>
#include <list>

////////////////////////////////////////////////////////////////////////////////

namespace HTTP
{

class ProcessingList
{
public:
	typedef std::function<bool(Request &, Response &, ProcessingList &)> EventCallback;

public:
	ProcessingList();
	~ProcessingList();

private:
	std::list<EventCallback> m_callbacks;
};

}