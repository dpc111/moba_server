#ifndef UDP_CONNECTION_H
#define UDP_CONNECTION_H

#include "type.h"
extern "c" {
	#include "rudp.h"	
}

class udp_network_t;

class udp_connection_t 
{
public:
	udp_connection_t(udp_network_t *network, sockaddr_in& addr);

	~udp_connection_t();

	udp_network_t* get_network() { return network_; }

	struct rudp* get_rudp() { return rudp_; }

	net_address_t& get_addr() { return peer_addr_; }

	int64 get_id() { return id_; }

	void rudp_send(const char *buffer, int sz) { return rudp_send(rudp_, buffer, sz)}
	
	struct rudp_package* rudp_update(const void * buffer, int sz, int tick) { return rudp_update(rudp_, buffer, sz, tick); }

	void rudp_recv() { input_stream_.read_rudp(); }

	void net_recv(const void *buff, int size, int tick);

	bool net_send();

private:
	udp_network_t *network_;

	struct rudp *rudp_;

	struct rudp_package send_list;

	net_address_t peer_addr_;

	int64 id_;

	udp_input_stream_t *input_stream_;

	udp_net_output_stream_t *net_output_stream_;
};

#endif