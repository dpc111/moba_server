#ifndef CHUNK_H
#define CHUNK_H

#include <stdlib.h>
#include <list>

class tcp_connection_t;

template <int size>
struct chunk_t {
	chunk_t() {
		conn_ = NULL;
		read_offset_ = 0;
		write_offset_ = 0;
	}

	~chunk_t() {
		read_offset_ = 0;
		write_offset_ = 0;
	}

	char* read_ptr() { 
		return buff_ + read_offset_;
	}

	char* write_ptr() { 
		return buff_ + write_offset_;
	}

	int read_size() {
		return write_offset_ - read_offset_;
	}

	int write_size() {
		return size - write_offset_;
	}

	int total_size() {
		return size;
	}

	tcp_connection_t *conn_;

	char buff_[size];

	int read_offset_;

	int write_offset_;
};

typedef chunk_t<1024> input_chunk_t;

typedef chunk_t<1024> output_chunk_t;

typedef std::list<input_chunk_t *> input_queue_t;

typedef std::list<output_chunk_t *> output_queue_t;

#endif