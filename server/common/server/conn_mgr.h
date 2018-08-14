#ifndef CONN_MGR_H
#define CONN_MGR_H

#include <map>

class tcp_connection_t;
class server_t;

class conn_mgr_t {
public:
	typedef struct {
		const char *ip;
		int port;	
		int sid;
	} server_addr_t;

	typedef std::map<int, server_addr_t *> server_addrs_t;

	typedef std::map<int, tcp_connection_t *> conns_t;

public:
	conn_mgr_t(server_t *server);

	~conn_mgr_t();

	void init();

	void verify(int sid, tcp_connection_t *conn);

	tcp_connection_t *get_conn(int sid);

	tcp_connection_t *connect_to(int sid);

	void on_disconnect(tcp_connection_t *conn);

private:
	server_t *server_;

	server_addrs_t server_addrs_;

	conns_t conns_;

	int conn_num_;
};

#endif