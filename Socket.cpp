#ifndef _SOCKET_CLASS_
#define _SOCKET_CLASS_
#include "System_Library.hpp"


namespace System 
{

#ifdef WINDOWS
	unsigned int Socket::_num_sockets = 0;
#endif // !WINDOWS
	void Socket::_socket(void)
	{
#ifdef WINDOWS
		this->_num_sockets++;
		if (this->_num_sockets == 1)
		{
			WSADATA wsaData;
			if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			{
				throw SocketException("[Construcor] Cannot initialize socket environment.");
			}
		}
#endif
	}

	Socket::Socket(void)
	{
		this->_socket();
	}

	Socket::Socket(SOCKET_TYPE socket_type)
	{
		this->_socket();

		this->_socket_type = socket_type;
		this->_isOpened = false;
		this->_isBinded = false;
	}

	void Socket::open(void)
	{
		if (!this->_isOpened)
		{
			if ((this->_socket_id = socket(PF_INET, this->_socket_type, 0)) == -1)
			{
				throw SocketException("[open] Cannot create socket");
			}
			this->_isOpened = true;
			this->_isBinded = true;
		}
	}

	void Socket::close(void)
	{
		if (this->_isOpened)
		{
#ifdef WINDOWS
			closesocket(this->_socket_id);
#else
			shutdown(this->_socket_id, SHUT_RDWR);
#endif
		}
		this->_isOpened = false;
		this->_isBinded = false;
	}

	void Socket::listen_on_port(PORT port)
	{
		if (this->_isBinded)
		{
			throw SocketException("[listen_on_port] Socket alread binded to a port, close the socket before to re-bind.");
		}

		if (!this->_isOpened)
		{
			this->open();
		}
		Address address(port);
	
		if (bind(this->_socket_id, (SOCKADDR*)&address, sizeof(SOCKADDR_IN)) == -1)
		{
			std::stringstream error;
			error << "[listen_on_port] with [port=" << port << "] Cannot bind socket";
			throw SocketException(error.str());
		}
		
		this->_isBinded = true;
	}

	Socket::~Socket(void)
	{
#ifdef WINDOWS
		this->_num_sockets--;
		if (this->_num_sockets == 0)
		{
			WSACleanup();
		}
#endif // WINDOWS
	}
}

#endif // !_SOCKET_CLASS_