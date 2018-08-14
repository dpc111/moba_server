#ifndef UDP_CONNECTION_H
#define UDP_CONNECTION_H

#include "type.h"
extern "c" {
	#include "rudp.h"	
}

class udp_connection_t 
{
public:
	udp_connection_t(sockaddr_in& addr);

	~udp_connection_t();

	struct rudp* get_rudp() { return rudp_; }

	int64 get_id() { return id_; }

	int rudp_recv(char buffer[MAX_PACKAGE]) { return rudp_recv(rudp_, buffer); }

	void rudp_send(const char *buffer, int sz) { return rudp_send(rudp_, buffer, sz)}
	
	struct rudp_package* rudp_update(const void * buffer, int sz, int tick) { return rudp_update(rudp_, buffer, sz, tick); }

private:
	struct rudp *rudp_;

	struct rudp_package send_list;

	net_address_t peer_addr_;

	int64 id_;

	udp_input_stream_t *input_stream_;

	udp_net_output_stream_t *net_output_stream_;
};

#endif