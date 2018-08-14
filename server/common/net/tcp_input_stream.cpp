#include <string.h>
#include <stdlib.h>
#include <typeinfo>
#include "chunk.h"
#include "tcp_connection.h"
#include "net_pool.h"
#include "log.h"

tcp_input_stream_t::tcp_input_stream_t(tcp_connection_t *conn) 
	: conn_(conn) {

}

tcp_input_stream_t::~tcp_input_stream_t() {

}

int tcp_input_stream_t::read_fd(void *ud, int fd) {
	tcp_connection_t *conn = (tcp_connection_t *)ud;
	input_queue_t& read_fd_buff = assign_read_fd_buff();
	iovec vecs[NUM_READ_IOVEC];
	int i = 0;
	for (input_queue_t::iterator it = read_fd_buff.begin(); it != read_fd_buff.end(); it++) {
		input_chunk_t *chunk = *it;
		vecs[i].iov_base = chunk->write_ptr();
		vecs[i].iov_len = chunk->write_size();
		i++;
		if (i == NUM_READ_IOVEC) {
			break;
		}
	}
	int n = ::readv(fd, vecs, i);
	// ERROR("%d", n);
	if (n <= 0) {
		WARN("read fd error");
		reset();
		// conn->del_event_read();
		return n;
	}
	int read_size = n;
	while (read_size > 0) {
		if (read_fd_buff.empty()) {
			return -1;
		}
		input_chunk_t *chunk = read_fd_buff.front();
		read_fd_buff.pop_front();
		// LOG("%d", buff_.size());
		assert(chunk);
		buff_.push_back(chunk);
		if (read_size < chunk->total_size()) {
			chunk->write_offset_ += read_size;
			read_size = 0;
			break;
		} else {
			chunk->write_offset_ += chunk->total_size();
			read_size -= chunk->total_size();
		}
	}
	size_ += n;
	return n;
}