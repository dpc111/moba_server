#include <string.h>
#include <stdlib.h>
#include <typeinfo>
#include "net_pool.h"
#include "log.h"

tcp_input_stream_t::tcp_input_stream_t() 
	: size_(0) {
	buff_.clear();
	ERROR("%d", buff_.size());
}

tcp_input_stream_t::~tcp_input_stream_t() {
	reset();
	buff_.clear();
}

int tcp_input_stream_t::read(void *buff, int size) {
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

bool tcp_input_stream_t::next(const void **data, int *size) {
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

void tcp_input_stream_t::backup(int num) {
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

void tcp_input_stream_t::finish() {
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

int tcp_input_stream_t::skip(int num) {
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

void tcp_input_stream_t::reset() {
	ERROR("reset");
	while (buff_.size() > 0) {
		input_chunk_t *chunk = buff_.front();
		input_chunk_free(chunk);
		buff_.pop_front();
	}
	size_ = 0;
}
