#ifndef NET_OUTPUT_STREAM_H
#define NET_OUTPUT_STREAM_H

#include "output_stream_t"

class tcp_connection_t;

class tcp_output_stream_t : output_stream_t
{
public:
	tcp_output_stream_t(tcp_connection_t *);

	~tcp_output_stream_t();

	tcp_connection_t *get_conn() { return conn_; }

	int write_fd(void *ud, int fd);
	
private:
	tcp_connection_t *conn_;
};

#endif
