udp_connection_t::udp_connection_t(sockaddr_in& addr)
	: peer_addr_(peer_addr)
	, id(0) {
	UDP_ADDR_ID(&addr, &id_);
	rudp_ = rudp_new(1, 100);
}

udp_connection_t::~udp_connection_t() {
	id = 0;
	rudp_delete(rudp_);
}