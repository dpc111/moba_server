#include <string.h>
#include <stdlib.h>
#include "log.h"
#include "output_stream.h"
#include "net_pool.h"

#define IOVEC_NUM 16

output_stream_t::output_stream_t()
	: size_(0) {
	buff_.clear();
}

output_stream_t::~output_stream_t() {
	reset();
	buff_.clear();
}

int output_stream_t::write(const void *buff, int size) {
	if (size <= 0) {
		return 0;
	}
	char *ptr = (char *)buff;
	while (true) {
		output_chunk_t *chunk;
		if (buff_.size() == 0 || buff_.back()->write_size() == 0) {
			chunk = output_chunk_alloc();
			buff_.push_back(chunk);
		} else {
			chunk = buff_.back();
		}
		int write_size = size < chunk->write_size() ? size : chunk->write_size();
		memcpy(chunk->write_ptr(), ptr, write_size);
		chunk->write_offset_ += write_size;
		ptr += write_size;
		size -= write_size;
		size_ += write_size;
		if (size <= 0) {
			break;
		}
	}
	return (char *)buff - ptr;	
}

bool output_stream_t::next(void **data, int *size) {
	output_chunk_t *chunk = NULL;
	if (buff_.size() == 0 || buff_.back()->write_size() == 0) {
		chunk = output_chunk_alloc();
		buff_.push_back(chunk);
	} else {
		chunk = buff_.back();
	}
	*data = chunk->write_ptr();
	*size = chunk->write_size();
	chunk->write_offset_ += *size;
	size_ += *size;
	return true;
}

void output_stream_t::backup(int size) {
	for (output_queue_t::reverse_iterator it = buff_.rbegin(); it != buff_.rend(); ++it) {
        output_chunk_t *chunk = *it;
        if(chunk->write_offset_ == 0) {
            continue;
        }
        size_t backup_size = size <= chunk->write_offset_ ? size : chunk->write_offset_;
        chunk->write_offset_ -= backup_size;
        size_ -= backup_size;
        size -= backup_size;
        if (size <= 0) {
            break;
        }
    }
}

void output_stream_t::append(output_chunk_t *chunk) {
	buff_.push_back(chunk);
	size_ += chunk->read_size();
}

void output_stream_t::reset() {
	while (!buff_.empty()) {
		output_chunk_t *chunk = buff_.front();
		output_chunk_free(chunk);
		buff_.pop_front();
	}
	size_ = 0;
}


