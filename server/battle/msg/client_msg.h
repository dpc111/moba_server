#ifndef CLIENT_MSG
#define CLIENT_MSG

#include "stdafx.h"
#include "message/messages.h"
#include "tcp_connection.h"

class client_msg_t {
public:
	client_msg_t();

	~client_msg_t();

	void c_login_hall(tcp_connection_t *conn, const battle_msg::c_login_hall& msg);

	void c_login(tcp_connection_t *conn, const battle_msg::c_login& msg);

	void default_message(int32 uid, const google::protobuf::Descriptor* des, const google::protobuf::Message *msg);
};

#endif