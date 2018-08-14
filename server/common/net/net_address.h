#ifndef NET_ADDRESS_H
#define NET_ADDRESS_H

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

class net_address_t 
{
public:
	net_address_t(int port) {
		memset(&addr_, 0, sizeof(addr_));	
		addr_.sin_family = AF_INET;
		addr_.sin_addr.s_addr = inet_addr("0, 0, 0, 0");
		addr_.sin_port = htons(port);
	}

	net_address_t(const char *ip, int port) {
		memset(&addr_, 0, sizeof(addr_));
		addr_.sin_family = AF_INET;
		addr_.sin_addr.s_addr = inet_addr(ip);
		addr_.sin_port = htons(port);
	}

	net_address_t(sockaddr_in addr) {
		addr_ = addr;
	}

	net_address_t() {
		
	}

	std::string to_ip() const {
		char buf[32];
		sprintf(buf, "%s", inet_ntoa(addr_.sin_addr));
		return buf;
	}

	std::string to_ip_port() const {
		char buf[32];
		sprintf(buf, "%s:%d", inet_ntoa(addr_.sin_addr), ntohs(addr_.sin_port));
		return buf;
	}

	int ip() const {
		return ntohl(addr_.sin_addr.s_addr);
	}

	int port() const {
		return ntohs(addr_.sin_port);
	}

public:
	struct sockaddr_in addr_;
};

#endif