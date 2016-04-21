#pragma once
#ifndef _SYSTEM_LIBRARY_HPP_
#define _SYSTEM_LIBRARY_HPP_

#if defined _WIN32 || defined _WIN64
	#define WINDOWS
#endif

#ifdef WINDOWS
	#include <windows.h>
	#pragma comment(lib, "ws2_32.lib")
#else
	#include <unistd.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
#endif

// C library
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

// ISO C++11 standard library
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include <exception>

#define SOCKET_MAX_BUFFER_LEN 1024

namespace System 
{
	typedef int SOCKET_TYPE;
	typedef unsigned int PORT;
	typedef int PROTOCOL;
	typedef std::string IP;
	typedef unsigned int LISTENER;

	typedef char* TAG;
	typedef char* MESSAGE;
#ifdef WINDOWS
	typedef unsigned int socket_t;
#else
	typedef int SOCKET;
	typedef socklen_t SOCKET_SIZE;
	typedef struct sockaddr SOCKET_ADDR;
	typedef struct sockaddr_in SOCKADDR_IN;
	typedef struct sockaddr SOCKADDR;
#endif
	

	class DataBase 
	{
		//__FUNCTION__
	};

	class SocketException : public std::exception 
	{
	private:
		std::string _error;
	public:
		SocketException(const std::string&);
		~SocketException() noexcept;
		virtual const char* what() const noexcept;
		friend std::ostream& operator<< (std::ostream&, SocketException&);
	};


	struct Address :protected SOCKADDR_IN
	{
	private:
		void _address(IP=INADDR_ANY, PORT=9999);
	public:
		Address();
		Address(IP, PORT);
		Address(PORT);
		Address(SOCKADDR_IN);
		Address(const Address&);

		IP ip(void);
		IP ip(IP);

		PORT port(void);
		PORT port(PORT);

		friend std::ostream& operator<< (std::ostream&, Address&);
	};

	class Socket
	{
	private:
#ifdef WINDOWS
		static unsigned int _num_sockets;
#endif // WINDOWS
		void _socket(void);
	protected:
		SOCKET _socket_id;
		SOCKET_TYPE _socket_type;
		bool _isOpened;
		bool _isBinded;
	public:
		Socket(void);
		Socket(SOCKET_TYPE);

		void open(void);
		void close(void);

		virtual void listen_on_port(PORT);
		~Socket(void);
	};


	class TCP : public Socket
	{	
	private:
		Address _address;
	public:
		TCP(void);
		TCP(const TCP&);

		IP ip(void);
		PORT port(void);
		Address address(void);

		void listen_on_port(PORT, LISTENER = 5);
		void connect_to(Address);

		TCP accept_client(void);

		template <class T> int send(const T*, size_t);
		template <class T> int receive(T*, size_t);
	};

	class Log
	{
	private:
		std::fstream _file;
		void _make_file(std::string);
		void print(TAG, MESSAGE);
		void print(MESSAGE);
	public:
		Log(TAG, MESSAGE);
		Log(MESSAGE);
		~Log();

	};
} // namespce System
#endif
