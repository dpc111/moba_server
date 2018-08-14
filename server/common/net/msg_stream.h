#include "google/protobuf/io/zero_copy_stream.h"
#include "tcp_input_stream.h"
#include "tcp_output_stream.h"

class msg_input_stream_t : public google::protobuf::io::ZeroCopyInputStream {
public:
	msg_input_stream_t(tcp_input_stream_t& stream, int size)
		: stream_(stream)
		, size_(size)
		, offset_(0) {
	} 

	~msg_input_stream_t() {
	}

	virtual bool Next(const void **data, int *size) {
		if (size_ <= 0) {
			return false;
		}
		*size = size_;
		if (stream_.next(data, size)) {
			size_ -= *size;
			offset_ += *size;
			return true;
		}
		return false;
	}

	virtual void BackUp(int num) {
		size_ += num;
		offset_ -= num;
		stream_.backup(num); 
	}

	virtual bool Skip(int num) {
		int skip = stream_.skip(num);
		size_ -= num;
		offset_ += skip;
		return skip == num;
	}

	virtual google::protobuf::int64 ByteCount() const {
		return offset_;
	}

private:
	tcp_input_stream_t& stream_;
	int size_;
	int offset_;
};

class msg_output_stream_t : public google::protobuf::io::ZeroCopyOutputStream {
public:
	msg_output_stream_t(tcp_output_stream_t& stream)
		: stream_(stream)
		, offset_(0) {
	}

	~msg_output_stream_t() {
	}

	virtual bool Next(void **data, int *size) {
		if (stream_.next(data, size)) {
			offset_ += *size;
			return true;
		}
		return false;
	}

	virtual void BackUp(int num) {
		offset_ -= num;
		stream_.backup(num);
	}

	virtual google::protobuf::int64 ByteCount() const {
		return offset_;
	}

private:
	tcp_output_stream_t& stream_;
	
	int offset_;
};