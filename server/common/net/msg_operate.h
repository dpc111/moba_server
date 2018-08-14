#ifndef NET_MESSAGE_H
#define NET_MESSAGE_H

#include "google/protobuf/message.h"

#define MSG_NAME_MAX_LEN 1024

class tcp_network_t;
class tcp_connection_t;

class msg_operate_t {
public:
	msg_operate_t(tcp_network_t *network);

	~msg_operate_t();

	google::protobuf::Message *gen_message(const char *name);

	void free_message(google::protobuf::Message *msg);

	void send(tcp_connection_t *conn, google::protobuf::Message& msg);

	void send_func(tcp_connection_t *conn, const char *funcname, const char *fmt, va_list vlist, int len);

	bool on_message(tcp_connection_t *conn);

private:
	tcp_network_t *network_;

	char msg_name_[MSG_NAME_MAX_LEN];
};

#endif
