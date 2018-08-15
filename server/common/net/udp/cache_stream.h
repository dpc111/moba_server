template <int size>
struct chunk_t {
	chunk_t() {
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

	char buff_[size];

	int read_offset_;

	int write_offset_;
};

typedef chunk_t<1024> input_chunk_t;
