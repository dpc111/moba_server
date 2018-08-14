#include "log.h"
#include "conn_mgr.h"
#include "tcp_network.h"
#include "tcp_connection.h"
#include "server.h"

conn_mgr_t::conn_mgr_t(server_t *server) {
	server_ = server;
	conn_num_ = 0;
}

conn_mgr_t::~conn_mgr_t() {
	server_ = NULL;
	conn_num_ = 0;
}

void conn_mgr_t::init() {

}

void conn_mgr_t::verify(int sid, tcp_connection_t *conn) {
	conns_t::iterator it = conns_.find(sid);
	if (it != conns_.end()) {
		ERROR("");
		return;
	}
	conns_[sid] = conn;
	conn->set_sid(sid);
	++conn_num_;
}

tcp_connection_t *conn_mgr_t::get_conn(int sid) {
	conns_t::iterator it = conns_.find(sid);
	if (it == conns_.end()) {
		return NULL;
	}
	return it->second;
}

tcp_connection_t *conn_mgr_t::connect_to(int sid) {
	conns_t::iterator it = conns_.find(sid);
	if (it != conns_.end()) {
		return NULL;
	}
	server_addrs_t::iterator it1 = server_addrs_.find(sid);
	if (it1 == server_addrs_.end()) {
		// ERROR("");
		return NULL;
	}
	server_addr_t *addr = it1->second;
	tcp_network_t *network = server_->get_network();
	tcp_connection_t *conn = network->connect_to(addr->ip, addr->port);
	return conn;
}

void conn_mgr_t::on_disconnect(tcp_connection_t *conn) {
	conns_t::iterator it = conns_.find(conn->get_sid());
	if (it == conns_.end()) {
		return;
	}
	tcp_connection_t *conn1 = it->second;
	conn1->set_sid(0);
	conns_.erase(it);
	--conn_num_;
}