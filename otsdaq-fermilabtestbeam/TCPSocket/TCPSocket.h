/*
 * TCPSocket.h
 *
 *  Created on: Jun 19, 2017
 *      Author: parilla
 */

#ifndef _ots_TCPSocket_h_
#define _ots_TCPSocket_h_

#include <netdb.h>
#include <mutex>
#include <string>
#include <vector>

const int MAXHOSTNAME    = 200;
const int MAXCONNECTIONS = 10;

struct addrinfo;

namespace otsftbf
{
class TCPSocket
{
  public:
	TCPSocket(int maxRecv = 5000);
	virtual ~TCPSocket(void);  // Server initialization

	bool create(void);
	bool bind(const int port);
	bool accept(TCPSocket&);
	bool isConnection(int timeout) const;

	// Client initialization
	bool connectTo(const std::string host, const int port);

	// Data Transimission
	bool send(const std::string&);
	int  recvFrom(std::string&);
	int  read(int bufferSize, std::string&);
	int  read(int bufferSize, std::string&, int flags);
	// For receiving two bytes at once and packing these two bytes as a uint16_t pushed
	// into a vector. Also have option of swapping bytes.
	int recv_block_16(std::vector<uint16_t>& d, int totalSize, bool swap = false);

	void set_non_blocking(const bool);

	bool  is_valid(void) const { return m_sock_ != -1; }
	void* get_in_addr(struct sockaddr* sa);
	bool  createSocket(std::string host, unsigned int port);
	bool  receiveOnSocket(void);
	int   clearReadSocket(void);

  protected:
	int         m_sock_;
	sockaddr_in m_addr_;
	int         m_maxRecv_;
	addrinfo    hints_;

	std::mutex
	    tcpMutex_;  // to make TCP socket thread safe
	                //	i.e. multiple threads can share a TCP socket and call receive()
};

} /* namespace ots */

#endif
