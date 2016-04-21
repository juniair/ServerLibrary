#ifndef _ADDRESS_STRCUT_CPP_
#define _ADDRESS_STRCUT_CPP_
#include "System_Library.hpp"
namespace System 
{

	void Address::_address(IP ip, PORT port)
	{
		this->sin_family = AF_INET;
		this->ip(ip);
		this->port(port);
	}

	Address::Address()
	{
		this->_address();
	}

	Address::Address(IP ip, PORT port)
	{
		this->_address(ip, port);
	}

	Address::Address(PORT port)
	{
		this->_address(INADDR_ANY, port);
	}

	Address::Address(SOCKADDR_IN address)
	{
		this->_address(inet_ntoa(address.sin_addr), address.sin_port);
	}

	Address::Address(const Address &address)
	{
		this->sin_family = address.sin_family;
		this->sin_addr = address.sin_addr;
		this->sin_port = address.sin_port;
	}


	IP Address::ip(void)
	{
		return inet_ntoa(this->sin_addr);
	}

	IP Address::ip(IP ip)
	{
#ifdef WINDOWS
		unsigned long address = inet_addr(ip.c_str());

		if (address == INADDR_NONE)
		{
			std::stringstream error;
			error << "[IP] with [IP=" << ip << "] Invalid IP address provided.";
			throw SocketException(error.str());
		}
		else
		{
			this->sin_addr.S_un.S_addr = address;
		}
#else
		if (inet_aton(ip, &this->sin_addr) == 0)
		{
			std::string tag = "[IP] with [IP=" + std::string(ip) + "] Invalid IP address provided.";
			throw SocketException(tag.c_str());
		}
#endif
	}

	PORT Address::port(void)
	{
		
		return ntohs(this->sin_port);
	}

	PORT Address::port(PORT port)
	{
		this->sin_port = htons(port);
		return this->port();
	}

	std::ostream& operator<< (std::ostream& out, Address& address)
	{
		out << address.ip() << " : " << address.port();
		return out;
	}

}
#endif // !_ADDRESS_STRCUT_CPP_