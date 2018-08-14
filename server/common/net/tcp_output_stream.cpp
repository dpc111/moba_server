#include <string.h>
#include <stdlib.h>
#include "log.h"
#include "tcp_output_stream.h"
#include "tcp_connection.h"
#include "net_pool.h"

#define IOVEC_NUM 16

tcp_output_stream_t::tcp_output_stream_t(tcp_connection_t *conn)
	: conn_(conn) {

}

tcp_output_stream_t::~tcp_output_stream_t() {

}

int tcp_output_stream_t::write_fd(void *ud, int fd) {
	tcp_connection_t *conn = (tcp_connection_t *)conn;
	iovec vecs[IOVEC_NUM];
	int i = 0;
	for (output_queue_t::iterator it = buff_.begin(); it != buff_.end(); ++it) {
		output_chunk_t *chunk = *it;
		vecs[i].iov_base = chunk->read_ptr();
		vecs[i].iov_len = chunk->read_size();
		i++;
		if (i >= IOVEC_NUM) {
			break;
		}
	}
	int n = ::writev(fd, vecs, i);
	//LOG("write fd size %d", n);
	if (n < 0) {
		return n;
	}
	int sz = n;
	while (!buff_.empty()) {
		output_chunk_t *chunk = buff_.front();
		if (sz < chunk->read_size()) {
			chunk->read_offset_ += sz;
			break;
		}
		sz -= chunk->read_size();
		output_chunk_free(chunk);
		buff_.pop_front();
	}
	size_ -= n;
	return n;
}
