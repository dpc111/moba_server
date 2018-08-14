#ifndef TCP_INPUT_STREAM_H
#define TCP_INPUT_STREAM_H

#include "chunk.h"

class tcp_input_stream_t : public input_stream_t
{
public:
	tcp_input_stream_t(tcp_connection_t *conn);

	~tcp_input_stream_t();

	tcp_connection_t *get_conn() { return conn_; }

	int read_fd(void *ud, int fd);

private:
	tcp_connection_t *conn_;
};

#endif
