#include "udp_network.h"

udp_network_t::udp_network_t()  {
	single_select_ = new single_select_t(0);
	fd_ = 0;
	conn_num_ = 0;
}

udp_network_t::~udp_network_t() {
	delete = single_select_;
	fd_ = 0;
	for (conn_map_t::iterator it = conns_.begin(); it != conns_.end(); ) {
		udp_connection_t *conn = it->second;
		delete conn;
		conn_map_t::iterator tmpit = conns_.erase(it);
		it = tmpit;
		--conn_num_;
	}
}

void udp_network_t::add_connection(udp_connection_t *conn) {
	int64 id = conn->get_id();
	conn_map_t::iterator it = conns_.find(id);
	if (it != conns_.end()) {
		return;
	}
	conns_[id] = conn;
	++conn_num_;
	return;
}

void udp_network_t::remove_connection(int64 id) {
	conn_map_t::iterator it = conns_.find(id);
	if (it == conns_.end()) {
		return;
	}
	udp_connection_t *conn = it->second;
	delete conn;
	conns_.erase(it);
	--conn_num_;
}

udp_connection_t* udp_network_t::get_connection(int64 id) {
	conn_map_t::iterator it = conns_.find(id);
	if (it == conns_.end()) {
		return NULL;
	}
	return it->second;
}

void udp_network_t::start(const char *ip, int port) {
	int fd_ = socket(AF_INET, SOCK_DGRAM, 0);
	if (bind(fd_, (struct socketaddr*)&addr_, sizeof(addr_)) < 0) {
		ERROR("bind fail");
		fd_ = 0;
		return;
	}
	single_select_->set_fd(fd_);
}

void udp_network_t::process() {
	if (!single_select_->working()) {
		return;
	}
	if (single_select_->read_check()) {

	}
	if (single_select_->write_check()) {

	}
}

void udp_network_t::read() {
	struct sockaddr_in peeraddr;
	socklen_t peerlen;
	int n;
	while (1) {
		int n = recvfrom(fd_, recv_buff_, MAX_PACKAGE, 0, (struct socketaddr *)&peeraddr, &peerlen);
		if (n > 0) {
			int64 id; 
			UDP_ADDR_ID(&peeraddr, &id);
			conn()
		} else if (n == 0){
			return;
		} else {
			ERROR("udp recv err");
			return;
		}
	}
}