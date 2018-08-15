#ifndef UDP_STREAM_H
#define UDP_STREAM_H

class udp_connection_t;

// net->rudp->this->logic
class udp_input_stream_t : public input_stream_t 
{
public:
	udp_input_stream_t(udp_connection_t *conn);
	
	~udp_input_stream_t();

	void read_rudp();
private:
	udp_connection_t *conn_; 
}

// logic->rudp->this->net
class udp_net_output_stream_t : public output_stream_t 
{
public:
	udp_net_output_stream_t(udp_connection_t *conn);
	
	~udp_net_output_stream_t();

	bool write_fd();

private:
	udp_connection_t *conn_; 
}

#endif