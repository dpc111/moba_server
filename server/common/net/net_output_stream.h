#ifndef NET_OUTPUT_STREAM_H
#define NET_OUTPUT_STREAM_H

#include "chunk.h"

class tcp_connection_t;

class net_output_stream_t
{
public:
	net_output_stream_t(tcp_connection_t *conn);

	~net_output_stream_t();

	tcp_connection_t *get_conn() { return conn_; }

	int size() const { return size_; }

	int write(const void *buff, int size);

	int write_fd(void *ud, int fd);
	
	bool next(void **data, int *size);
	
	void backup(int size);

	void append(output_chunk_t *chunk);

	void reset();

private:
	tcp_connection_t *conn_;
	
	output_queue_t buff_;

	int size_;
};

#endif
