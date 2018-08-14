// #define CLIENT
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef CLIENT

#include "stdio.h"
#include "log.h"
#include "thread.h"
#include "net_address.h"
#include "tcp_network.h"
#include "tcp_connection.h"

#include "timer.h"
#include "timestamp.h"
#include "server.h"
#include "lua_frame.h"
#include "message/messages.h"

server_t server("0.0.0.0", 7769);
tcp_connection_t *conn;

class test_timer : public timer_handler_t {
public:
	virtual void handle_timeout(timer_handle_t handle, void *user) {
		int res;
		bool ok = server.get_lua_frame()->call_func("test_add", "ii:i", 1, 2, &res);
		for (int i = 1; i < 200; i++) 
			server.send_func(conn, "msg_func", "iis", 1, 8, "dpc");
		battle::s2c_join msg;
		msg.set_uid(11111);
		msg.set_name("dpcddy");
		msg.set_icon("iconurl");
		server.send(conn, msg);
		assert(ok);
	}
};

int main() {
	server.get_lua_frame()->load_script("test.lua");
	server.get_lua_frame()->run_script();
	server.start();
	test_timer timer;
	server.register_timer(&timer, NULL, 10000, 2000);

	conn = server.connect_to("127.0.0.1", 7768);

	server.process();

	return 1;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
#else

#include "stdio.h"
#include "log.h"
#include "thread.h"
#include "net_address.h"
#include "tcp_network.h"
#include "tcp_connection.h"

#include "timer.h"
#include "timestamp.h"
#include "server.h"
#include "lua_frame.h"
#include "message/messages.h"

server_t server("0.0.0.0", 7768);

class test_timer : public timer_handler_t {
public:
	virtual void handle_timeout(timer_handle_t handle, void *user) {
		int res;
		bool ok = server.get_lua_frame()->call_func("test_add", "ii:i", 1, 2, &res);
		printf(".....%d\n", res);
		assert(ok);
	}

	void test_msg(tcp_connection_t *conn, const battle::s2c_join& msg) {
		ERROR("%d", msg.uid());
		ERROR("%s", msg.name().c_str());
		ERROR("%s", msg.icon().c_str());
	}
};

int main() {
	server.get_lua_frame()->load_script("test.lua");
	server.get_lua_frame()->run_script();
	server.start();
	test_timer timer;
	// server.register_timer(&timer, NULL, 10000, 10000);
	server.register_net_message<battle::s2c_join>(std::tr1::bind(&test_timer::test_msg, &timer, std::tr1::placeholders::_1, std::tr1::placeholders::_2));

	server.process();

	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
#endif