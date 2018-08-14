#ifndef UDP_NETWORK_H
#define UDP_NETWORK_H

#include "type.h"
#include "single_select.h"
#include "udp_connection.h"
#include <map>

#define UDP_ADDR_ID(addr, id) \
memcpy(id, &(addr->sin_port), sizeof(addr->sin_port)); \
memcpy(id + sizeof(int), &(addr->in_addr), sizeof(addr->sin_addr));

class udp_network_t {
public:
	typedef std::map<int64, udp_connection_t *> conn_map_t;

public:
	udp_network_t();

	~udp_network_t();

	void add_connection(udp_connection_t *conn);

	void remove_connection(int64 id);

	udp_connection_t* get_connection(int64 id);

	void start(const char *ip, int port);

	void process();

private:
	void read();

	void write();

private:
	conn_map_t conns_;

	int conn_num_;

	net_address_t addr_;

	int fd_;

	single_select_t *single_select_;

	char recv_buff_[MAX_PACKAGE];
};

#endif