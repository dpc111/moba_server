#ifndef DISPATCH_MSG
#define DISPATCH_MSG

class service_t;
class client_msg_t;
class server_msg_t;

class dispatch_msg_t {
public:
	dispatch_msg_t(service_t *service);

	~dispatch_msg_t();

	void register_client_msg();

	void register_server_msg();

private:
	service_t *service_;

	client_msg_t *client_msg_;

	server_msg_t *server_msg_;
};

#endif