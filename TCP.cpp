#ifndef _TCP_CPP_
#define _TCP_CPP_
#include "System_Library.hpp"
namespace System 
{
	TCP::TCP(void) : Socket(SOCK_STREAM)
	{
	}

	TCP::TCP(const TCP &tcp) : Socket()
	{
		this->_socket_id = tcp._socket_id;
		this->_isOpened = tcp._isOpened;
		this->_isBinded = tcp._isBinded;
	}

	IP TCP::ip(void)
	{
		return this->_address.ip();
	}

	PORT TCP::port(void)
	{
		return this->_address.port();
	}

	Address TCP::address(void)
	{
		return Address(this->_address);
	}

	void TCP::listen_on_port(PORT port, LISTENER listener)
	{
		Socket::listen_on_port(port);

		if (listen(this->_socket_id, listener) != 0)
		{
			std::stringstream error;
			error << "[listen_on_port] with [port=" << port << "] [listeners=" << listener << "] Cannot bind socket";
			throw SocketException(error.str());
		}
	}

	void TCP::connect_to(Address address)
	{
		if (this->_isBinded)
		{
			throw SocketException("[connect_to] Socket already binded to a port, use another socket");
		}

		if (!this->_isOpened)
		{
			this->open();
		}

		if(connect(this->_socket_id, (SOCKADDR*)&address, sizeof(SOCKADDR_IN)) < 0)
		{
			std::stringstream error;
			error << "[connect_to] with [address=" << address << "] Cannot connect to the specified address";
			throw SocketException(error.str());
		}
				
		this->_isBinded = true;
	}

	template <class T>
	int TCP::send(const T* buffer, size_t len)
	{
		if (!this->_isBinded)
		{
			throw SocketException("[sned] Socket not binded.");
		}
		
		if (!this->_isOpened)
		{
			throw SocketException("[sned] Socket not opened.");
		}

		len *= sizeof(T);
		if (len > (SOCKET_MAX_BUFFER_LEN * sizeof(T)))
		{
			std::stringstream error;
			error << "[send] [len=" << len << "] Data length higher then max buffer len (" << SOCKET_MAX_BUFFER_LEN << ")";
			throw SocketException(error.str());
		}

		int result;
		if ((result = ::send(this->_socket_id, (const char*)buffer, len, 0)) == -1)
		{
			throw SocketException("[send] Cannot send.");
		}

		return result;
	}

	template <class T>
	int TCP::receive(T* buffer, size_t len)
	{

		if (!this->_isBinded)
		{
			throw SocketException("[sned] Socket not binded.");
		}

		if (!this->_isOpened)
		{
			throw SocketException("[sned] Socket not opened.");
		}

		len *= sizeof(T);
		if (len > (SOCKET_MAX_BUFFER_LEN * sizeof(T)))
		{
			std::stringstream error;
			error << "[send] [len=" << len << "] Data length higher then max buffer len (" << SOCKET_MAX_BUFFER_LEN << ")";
			throw SocketException(error.str());
		}

		int result;
		if ((result = recv(this->_socket_id, (const char*)buffer, len, 0)) == -1)
		{
			throw SocketException("[receive] Cannot receive.");
		}

		return result;
	}
}
#endif // !_TCP_CPP_