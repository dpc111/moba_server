#include <string.h>
#include <stdlib.h>
#include <typeinfo>
#include "chunk.h"
#include "tcp_connection.h"
#include "net_pool.h"
#include "log.h"

net_input_stream_t::net_input_stream_t(tcp_connection_t *conn) 
	: conn_(conn)
	, size_(0) {
	buff_.clear();
	ERROR("%d", buff_.size());
}

net_input_stream_t::~net_input_stream_t() {
	reset();
	buff_.clear();
}

int net_input_stream_t::read(void *buff, int size) {
	char *ptr = (char *)buff;
	for (input_queue_t::iterator it = buff_.begin(); it != buff_.end(); ++it) {
		input_chunk_t *chunk = *it;
		int read_size = size < chunk->read_size() ? size : chunk->read_size(); 
		memcpy(ptr, chunk->read_ptr(), read_size);
		ptr += read_size;
		size -= read_size;
		chunk->read_offset_ += read_size;
		size_ -= read_size;
		// need backup
		// if (chunk->read_size() == 0) {
		// 	if (buff_.size() == 1) {
		// 		chunk->read_offset_ = 0;
		// 		chunk->write_offset_ = 0;
		// 		break;
		// 	} else {
		// 		input_queue_t::iterator it1 = it;
		// 		++it;
		// 		buff_.erase(it1);
		// 		input_chunk_free(chunk);
		// 	}
		// }
		if (size <= 0) {
			break;
		}
	}
	return ptr - (char *)buff;
}

int net_input_stream_t::read_fd(void *ud, int fd) {
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

bool net_input_stream_t::next(const void **data, int *size) {
	if (size_ == 0) {
		return false;
	}
	for (input_queue_t::iterator it = buff_.begin(); it != buff_.end(); ++it) {
		input_chunk_t *chunk = *it;
		if (chunk->read_size() > 0)	{
			*data = chunk->read_ptr();
			*size = *size <= chunk->read_size() ? *size : chunk->read_size();
			chunk->read_offset_ += *size;
			size_ -= *size;
			return true;
		}
	}
	return false;
}

void net_input_stream_t::backup(int num) {
	if (num == 0) {
		return;
	}	
	for (input_queue_t::reverse_iterator it = buff_.rbegin(); it != buff_.rend(); ++it) {
		input_chunk_t *chunk = *it;
		if (chunk->read_offset_ == 0) {
			continue;
		}
		ERROR("%d", num); 
		ERROR("%d", chunk->read_offset_ ); 
		int backup_size = num <= chunk->read_offset_ ? num : chunk->read_offset_;
		chunk->read_offset_ -= backup_size;
		size_ += backup_size;
		num -= backup_size;
		if (num == 0) {
			break;
		}
	}
}

void net_input_stream_t::finish() {
	for (input_queue_t::iterator it = buff_.begin(); it != buff_.end();) {
		input_chunk_t *chunk = *it;
		if (chunk->read_offset_ == chunk->write_offset_) {
			// if (buff_.size() == 1) {
			// 	chunk->read_offset_ = 0;
			// 	chunk->write_offset_ = 0;
			// 	break;
			// } else {
			// 	input_queue_t::iterator it1 = it;
			// 	it = buff_.erase(it1);
			// 	input_chunk_free(chunk);
			// 	continue;
			// }
			input_queue_t::iterator it1 = it;
			it = buff_.erase(it1);
			input_chunk_free(chunk);
		} else {
			++it;
		}
	}
}

int net_input_stream_t::skip(int num) {
	if (size_ <= 0) {
		return 0;
	}
	int backup = num;
	for (input_queue_t::iterator it = buff_.begin(); it != buff_.end(); ++it) {
		input_chunk_t *chunk = *it;
		if (chunk->read_size() <= 0) {
			continue;
		}
		int skip_size = num <= chunk->read_size() ? num : chunk->read_size();
		chunk->read_offset_ += skip_size;
		size_ -= skip_size;
		num -= skip_size;
		if (num <= 0) {
			break;
		}	
	}
	return backup - num;
}

void net_input_stream_t::reset() {
	ERROR("reset");
	while (buff_.size() > 0) {
		input_chunk_t *chunk = buff_.front();
		input_chunk_free(chunk);
		buff_.pop_front();
	}
	size_ = 0;
}
