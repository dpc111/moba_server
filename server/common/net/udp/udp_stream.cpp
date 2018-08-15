#include "udp_stream.h"
#include "udp_connection.h"
#include "net_pool.h"
#include "rudp"

udp_input_stream_t::udp_input_stream_t(udp_connection_t *conn)
	:conn_(conn) {

}
	
udp_input_stream_t::~udp_input_stream_t() {
	conn_ = NULL;
}

void udp_input_stream_t::read_rudp() {
	struct rudp *U = conn->get_rudp();
	while (rudp_check_new_data(U) > 0) {
		chunk_t *chunk = input_chunk_alloc();
		int n = rudp_recv(conn_->get_rudp(), chunk->write_ptr());
		if (n < 0) {
			ERROR("");
			return;
		}
		if (n == 0) {
			return;
		}
		assert(n < chunk->total_size());
		chunk->write_offset_ += n;
		size_ += n;
		buff_.push_back(chunk);
	}
}


udp_net_output_stream_t::udp_net_output_stream_t(udp_connection_t *conn) 
	: conn_(conn) {

}
	
udp_net_output_stream_t::~udp_net_output_stream_t() {
	conn_ = NULL;
}

bool udp_net_output_stream_t::write_fd() {
	if (size_ < 0) {
		return true;
	}
	int fd = conn_->get_network()->get_fd();
	int len = sizeof(SOCKADDR_IN);
	int sendsz = 0;
	int n = 0;
	for (output_queue_t::iterator it = buff_.begin(); it != buff_.end(); ++it) {
		output_chunk_t *chunk = *it;
		if (chunk->read_size() <= 0) {
			continue;
		}
		n = sendto(fd, chunk->read_ptr(), chunk->read_size(), 0, (SOCKADDR*)&conn_->get_addr(), &len);
		if (n <= 0) {
			break;
		}
		sendsz += n;
	}
	while (!buff_.empty()) {
		output_chunk_t *chunk = buff_.front();
		if (sendsz < chunk->read_size()) {
			chunk->read_offset_ += sendsz;
			break;
		}
		sendsz -= chunk->read_size();
		output_chunk_free(chunk);
		buff_.pop_front();
	}
	size_ -= sendsz;
	if (n <= 0) {
		return false;
	}
	return true;
}