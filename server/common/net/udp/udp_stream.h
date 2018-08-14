#ifndef UDP_STREAM_H
#define UDP_STREAM_H

// rudp->this->logic
class udp_input_stream_t : public input_stream_t 
{
public:
	udp_input_stream_t();
	
	~udp_input_stream_t();

	void read_rudp();
}

// // del
// // logic->this->rudp
// class udp_output_stream_t : public output_stream_t 
// {
// public:
// 	udp_output_stream_t();
	
// 	~udp_output_stream_t();
// }

// // del
// // net->this->rudp
// class udp_net_input_stream_t : public input_stream_t 
// {
// public:
// 	udp_net_input_stream_t();
	
// 	~udp_net_input_stream_t();
// }

// rudp->this->net
class udp_net_output_stream_t : public output_stream_t 
{
public:
	udp_net_output_stream_t();
	
	~udp_net_output_stream_t();

	void write_fd(); 
}


#endif