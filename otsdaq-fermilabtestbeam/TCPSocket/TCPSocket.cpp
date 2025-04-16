/*
 * TCPSocket.cpp
 *
 *  Created on: Jun 19, 2017
 *      Author: parilla
 */

#include "TCPSocket.h"
#include <fcntl.h>
#include <iostream>

#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "otsdaq/Macros/InterfacePluginMacros.h"

#include <arpa/inet.h>
using namespace otsftbf;

//========================================================================================================================
TCPSocket::TCPSocket(int maxRecv) : m_sock_(-1), m_maxRecv_(maxRecv) {}

//========================================================================================================================
TCPSocket::~TCPSocket()
{
	//__MOUT__ << "CLOSING THE TCP SOCKET!" << std::endl;

	// lockout other receivers for the remainder of the scope
	std::lock_guard<std::mutex> lock(tcpMutex_);

	if(m_sock_ != -1)
	{
		std::cout << "Socket is valid... closing now!" << std::endl;
		::close(m_sock_);
	}
}

//========================================================================================================================
bool TCPSocket::create()
{
	// lockout other receivers for the remainder of the scope
	std::lock_guard<std::mutex> lock(tcpMutex_);

	(m_sock_ = socket(AF_UNSPEC, SOCK_STREAM, 0));
	//	  (m_sock_ = socket ( AF_INET, SOCK_STREAM, 0 ));

	// m_sock_ = socket ( AF_INET, SOCK_STREAM, 0 );

	if(!is_valid())
		return false;

	// TIME_WAIT - argh
	int on = 1;
	if(setsockopt(m_sock_, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(on)) == -1)
		return false;

	return true;
}

//========================================================================================================================
bool TCPSocket::createSocket(std::string host, unsigned int port)
{
	// lockout other receivers for the remainder of the scope
	std::lock_guard<std::mutex> lock(tcpMutex_);

	memset(&hints_, 0, sizeof(hints_));

	int              rv;
	struct addrinfo *servinfo, *p;
	char             s[INET6_ADDRSTRLEN];
	hints_.ai_family   = AF_UNSPEC;
	hints_.ai_socktype = SOCK_STREAM;

	if((rv = getaddrinfo(
	        host.c_str(), std::to_string(port).c_str(), &hints_, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return true;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next)
	{
		if((m_sock_ = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			perror("client: socket");
			continue;
		}

		if(connect(m_sock_, p->ai_addr, p->ai_addrlen) == -1)
		{
			perror("client: connect");
			close(m_sock_);
			continue;
		}

		break;
	}

	if(p == NULL)
	{
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr*)p->ai_addr), s, sizeof s);
	printf("client: connecting to %s\n", s);

	freeaddrinfo(servinfo);  // all done with this structure
	std::cout
	    << "\n TCPSocket::createSocket(std::string host, unsigned int port) finished."
	    << std::endl;

	return true;
}

//========================================================================================================================
bool TCPSocket::receiveOnSocket()
{
	// lockout other receivers for the remainder of the scope
	std::lock_guard<std::mutex> lock(tcpMutex_);

	std::vector<char> buf_v(m_maxRecv_);
	char*             buf = &buf_v[0];
	int               numbytes;

	if((numbytes = recv(m_sock_, buf, m_maxRecv_ - 1, 0 /*MSG_DONTWAIT*/)) == -1)
	{
		perror("recv");
		return false;
	}

	buf[numbytes] = '\0';

	printf("client: received '%s'\n", buf);

	return true;
}

//========================================================================================================================
bool TCPSocket::bind(const int port)
{
	// lockout other receivers for the remainder of the scope
	std::lock_guard<std::mutex> lock(tcpMutex_);

	if(!is_valid())
	{
		return false;
	}

	m_addr_.sin_family      = AF_INET;
	m_addr_.sin_addr.s_addr = INADDR_ANY;
	m_addr_.sin_port        = htons(port);

	int bind_return = ::bind(m_sock_, (struct sockaddr*)&m_addr_, sizeof(m_addr_));

	if(bind_return == -1)
		return false;

	return true;
}

//
// bool TCPSocket::listen() const
//{
//	//lockout other receivers for the remainder of the scope
//
//	if ( ! is_valid() )
//	{
//		return false;
//	}
//
//	int listen_return = ::listen ( m_sock_, MAXCONNECTIONS);
//
//
//	if ( listen_return == -1 )
//		return false;
//
//
//	return true;
//}

//========================================================================================================================
bool TCPSocket::accept(TCPSocket& new_socket)
{
	// lockout other receivers for the remainder of the scope
	std::lock_guard<std::mutex> lock(tcpMutex_);

	int addr_length = sizeof(m_addr_);

	new_socket.m_sock_ = ::accept(m_sock_, (sockaddr*)&m_addr_, (socklen_t*)&addr_length);

	if((new_socket.m_sock_ <= 0))
		return false;
	else
		return true;
}

//========================================================================================================================
bool TCPSocket::isConnection(int timeout) const
{
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(m_sock_, &rfds);
	struct timeval tv;
	tv.tv_sec  = timeout;
	tv.tv_usec = 0;

	int retval;
	retval = select(m_sock_ + 1, &rfds, NULL, NULL, &tv);

	if(retval == -1)
		return false;
	else if(retval)
		return true;
	else
		return false;
}

//========================================================================================================================
bool TCPSocket::send(const std::string& s)
{
	// lockout other receivers for the remainder of the scope
	std::lock_guard<std::mutex> lock(tcpMutex_);

	std::cout << "\n\n TCPSocket::send : Sending message to TCPSocket! : " << s
	          << std::endl;

	// Write a text string to the socket (s)
	// All messages require a line feed at the end
	// Silently add a line feed if needed
	std::string lineFeed("\n");
	if(s[s.size() - 1] == lineFeed[0])
	{
		lineFeed = "";
	}

	int status =
	    ::send(m_sock_, (s + lineFeed).c_str(), s.size() + lineFeed.size(), MSG_NOSIGNAL);

	if(status == -1)
		return false;
	else
		return true;
}

//========================================================================================================================
int TCPSocket::recvFrom(std::string& s)
{
	// lockout other receivers for the remainder of the scope
	std::lock_guard<std::mutex> lock(tcpMutex_);

	std::vector<char> buf_v(m_maxRecv_ + 1);
	char*             buf = &buf_v[0];

	s = "";

	memset(buf, 0, m_maxRecv_ + 1);

	int status = ::recv(m_sock_, buf, m_maxRecv_, 0);

	if(status == -1)
	{
		std::cout << "status == -1   errno == " << errno
		          << "  in Socket::recv\n";  // TODO Exception/logger
		return 0;
	}
	else if(status == 0)
	{
		return 0;
	}
	else
	{
		s = buf;
		return status;
	}
}

//========================================================================================================================
int TCPSocket::read(int bufferSize, std::string& s)
{
	int b = 0;
	b     = read(bufferSize, s, 0 /*No flags*/);
	return b;
}

//========================================================================================================================
int TCPSocket::read(int bufferSize, std::string& s, int flags)
{
	// lockout other receivers for the remainder of the scope
	std::lock_guard<std::mutex> lock(tcpMutex_);

	// std::cout << "\n\n\n TCPSocket::read : Attempting to read data! \n\n" << std::endl;

	//	//set timeout period for select()
	//		timeout_.tv_sec  = timeoutSeconds;
	//		timeout_.tv_usec = timeoutUSeconds;
	//
	//		FD_ZERO(&fileDescriptor_);
	//		FD_SET(socketNumber_  , &fileDescriptor_);
	//		select(socketNumber_+1, &fileDescriptor_, 0, 0, &timeout_);
	//
	//		if(FD_ISSET(socketNumber_, &fileDescriptor_))

	// char buf [bufferSize + 1 ];

	s = "";

	// memset ( buf, 0, bufferSize + 1 );

	s.resize(
	    bufferSize);  // NOTE: this is inexpensive according to Lorenzo/documentation
	                  // in C++11 (only increases size once and doesn't decrease size)
	int status = ::recv(m_sock_, &s[0], bufferSize, 0);
	if(status > 0)  // status is number of bytes received
		s.resize(status);
	else  // error or no response
		s.resize(0);

	//	char msg[100];
	//
	//	{
	//		char msg[100];
	//
	//		std::stringstream ss;
	//		ss << "TCPSocket::read RECV Response: ";
	//		ss << "Size of Response: " << status%4 << " " << status << std::endl;
	//		for(int i=0;i<status;++i)
	//		{
	//			sprintf(msg,"0x%2.2x",(unsigned int)s[i]);
	//			ss << msg << " ";
	//		}
	//		std::cout << "\n" << ss.str() << std::endl;
	//	}

	//  std::cout << "int TCPSocket::read (int bufferSize, std::string& s ) const: buf =
	//  "<< buf << std::endl;

	if(status == -1)
	{
		std::cout << "status == -1   errno == " << errno
		          << "  in Socket::recv\n";  // TODO Exception/logger
		return 0;
	}
	else if(status == 0)
	{
		return 0;
	}
	else
	{
		// s = buf;
		//		{
		//			char msg[100];
		//
		//			std::stringstream ss;
		//			ss << "TCPSocket::read RECV String Response: ";
		//			for(unsigned int i=0;i<s.size();++i)
		//			{
		//				sprintf(msg,"0x%2.2x",(unsigned int)s[i]);
		//				//ss << msg << " ";
		//			}
		//			//std::cout << "\n" << ss.str() << std::endl;
		//		}
		return status;
	}
}

// Receive two bytes at a time from the socket and pack into a vector of uint16.
// Also allow for the option to swap the byte order ie first byte becomes bits 7-15 and
// second 0-7.  TO DO : ADD A TO DO.
//========================================================================================================================
int TCPSocket::recv_block_16(std::vector<uint16_t>& d, int totalSize, bool swap)
{
	// lockout other receivers for the remainder of the scope
	std::lock_guard<std::mutex> lock(tcpMutex_);

	// Receive two bytes at a time
	int                        recvSize = 2;
	std::vector<unsigned char> buf_v(recvSize);
	unsigned char*             buf = &buf_v[0];
	int                        status;
	d.clear();

	memset(buf, 0, recvSize);

	int bytesReceived = 0;
	while(bytesReceived < totalSize)
	{
		int status = ::recv(m_sock_, buf, recvSize, 0);
		bytesReceived += recvSize;

		if(status == -1)
		{
			std::cout << "status == -1   errno == " << errno
			          << "  in Socket::recv\n";  // TODO Exception
			return 0;
		}
		else if(status == 0)
		{
			return 0;
		}
		else
		{
			if(swap)
			{
				unsigned char b0 = buf[0];
				unsigned char b1 = buf[1];
				buf[0]           = b1;
				buf[1]           = b0;
			}
			uint16_t val = buf[1] << 8 | buf[0];
			// std::cerr << "val = " << std::hex << val << std::dec << " : buf[1] (msb) =
			// " << buf[1] << " : buf[0] (lsb) = " << buf[0] << std::endl; //TODO REMOVE
			d.push_back(val);
		}
	}
	return status;
}

//========================================================================================================================
bool TCPSocket::connectTo(const std::string host, const int port)
{
	// lockout other receivers for the remainder of the scope
	std::lock_guard<std::mutex> lock(tcpMutex_);

	if(!is_valid())
		return false;

	// Set port
	bzero((char*)&m_addr_, sizeof(m_addr_));
	m_addr_.sin_family = AF_INET;
	m_addr_.sin_port   = htons(port);

	// Set host
	struct hostent* server = gethostbyname(host.c_str());
	if(!server)
		return false;
	bcopy((char*)server->h_addr, (char*)&m_addr_.sin_addr.s_addr, server->h_length);
	// int status = inet_pton ( AF_INET, host.c_str(), &m_addr_.sin_addr );

	if(errno == EAFNOSUPPORT)
		return false;

	int status = ::connect(m_sock_, (struct sockaddr*)&m_addr_, sizeof(m_addr_));

	if(status == 0)
		return true;
	else
		return false;
}

//========================================================================================================================
void TCPSocket::set_non_blocking(const bool b)
{
	int opts;

	opts = fcntl(m_sock_, F_GETFL);

	if(opts < 0)
	{
		return;
	}

	if(b)
		opts = (opts | O_NONBLOCK);
	else
		opts = (opts & ~O_NONBLOCK);

	fcntl(m_sock_, F_SETFL, opts);
}

//========================================================================================================================
void* TCPSocket::get_in_addr(struct sockaddr* sa)
{
	if(sa->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

//========================================================================================================================
int TCPSocket::clearReadSocket()
{
	//	std::string dummerReceiveBuffer;
	//	int cnt = 0;
	//	std::string buf;
	//	int bufferSize = 100;
	//	while(read(4096, buf, ) != 0) ++cnt;
	//	return cnt;

	//
	//	struct timeval time;
	//	time.tv_sec = 1;
	//	time.tv_usec = 0;

	//	int retime = select(1, &rfds, NULL, NULL, time);
	int cnt = 0;
	while(isConnection(1) != 0)
		++cnt;

	return cnt;
}
