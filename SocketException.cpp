#ifndef _SOCKET_EXCEPTION_CPP_
#define _SOCKET_EXCEPTION_CPP_
#include "System_Library.hpp"
namespace System 
{
	SocketException::SocketException(const std::string& error)
	{
		this->_error = error;
	}
	
	const char* SocketException::what() const noexcept
	{
		Log((char*) this->_error.c_str());
		return this->_error.c_str();
	}

	std::ostream& operator<< (std::ostream& out, SocketException& e)
	{
		out << e.what();
		return out;
	}

	SocketException::~SocketException() noexcept
	{
	}
}
#endif // !_SOCKET_EXCEPTION_CPP_