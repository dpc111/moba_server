udp_connection_t::udp_connection_t(udp_network_t *network,sockaddr_in& addr)
	: network_(network)
	, peer_addr_(peer_addr)
	, id(0) {
	UDP_ADDR_ID(&addr, &id_);
	rudp_ = rudp_new(1, 100);
}

udp_connection_t::~udp_connection_t() {
	id = 0;
	rudp_delete(rudp_);
}

void udp_connection_t::net_recv(const void *buff, int size, int tick) {
	struct rudp_package *sendpk = rudp_update(rudp_, buff, size, tick);
	while (sendpk) {
		net_output_stream_.write(sendpk->buff, sendpk->size);
		sendpk = sendpk->next;
	}
}

bool udp_connection_t::net_send() {
	return net_output_stream_->write_fd();
}