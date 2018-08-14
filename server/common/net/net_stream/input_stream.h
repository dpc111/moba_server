#ifndef NET_INPUT_STREAM_H
#define NET_INPUT_STREAM_H

#include "chunk.h"

class net_input_stream_t
{
public:
	net_input_stream_t(tcp_connection_t *conn);

	~net_input_stream_t();

	int size() const { return size_; }

	int read(void *buff, int size);

	bool next(const void **data, int *size);

	void backup(int num);

	void finish();

	int skip(int num);

	void reset();

private:
	input_queue_t buff_;

	int size_;
};

#endif
