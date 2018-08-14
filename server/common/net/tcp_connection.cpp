#include <sys/socket.h>
#include <netinet/tcp.h>
#include "tcp_connection.h"

tcp_connection_t::tcp_connection_t(int fd, sockaddr_in& peer_addr) 
	: input_stream_(this)
	, output_stream_(this)
	, fd_(fd)
	, state_(CNT_STATE_DISCONNECTED)
	, peer_addr_(peer_addr)
	, closed_(false)
	, sid_(0) {
}

tcp_connection_t::tcp_connection_t()
	: input_stream_(this)
	, output_stream_(this)
	, state_(CNT_STATE_DISCONNECTED)
	, closed_(false)
	, sid_(0) {
}

tcp_connection_t::~tcp_connection_t() {
	closed_ = true;
	sid_ = 0;
	fd_ = -1;
}

bool tcp_connection_t::reliable() {
	int err = evutil_socket_geterror(fd_);
	if (err == 0 || err == EINTR || err == EAGAIN) {
		return true;
	}
	return false; 
}

void tcp_connection_t::set_events(event_base *ev_base, event_callback_fn read_fn, event_callback_fn write_fn) {
	ev_read_ = event_new(ev_base, fd_, EV_READ | EV_PERSIST, read_fn, this);
	// ev_write_ = event_new(ev_base, fd_, EV_WRITE, write_fn, this);
	ev_write_ = event_new(ev_base, fd_, EV_WRITE | EV_PERSIST, write_fn, this);
	event_add(ev_read_, NULL);
	ev_read_add_ = true;
	// socket关闭后等到套接字内数据发送完成后才真正关闭连接
	linger lin;
	lin.l_onoff = 1;
	lin.l_linger = 0;
	setsockopt(fd_, SOL_SOCKET, SO_LINGER, (const char *)&lin, sizeof(linger));
	// 不会将小包进行拼接成大包再进行发送
	bool nodelay = true;
	setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, (const char *)&nodelay, sizeof(char));
}

void tcp_connection_t::unset_events() {
	event_free(ev_read_);
	event_free(ev_write_);
	ev_read_ = NULL;
	ev_write_ = NULL;
}

void tcp_connection_t::add_event_write() {
	// ERROR("add event write");
	if (!ev_write_add_) {
	// ERROR("add event write");
		ev_write_add_ = true;
		event_add(ev_write_, NULL);
	}
}

void tcp_connection_t::del_event_write() {
	// ERROR("del event write");
	if (ev_write_add_) {
	// ERROR("del event write");
		ev_write_add_ = false;
		event_del(ev_write_);
	}
}

void tcp_connection_t::connect_destroy() {
	// ::shutdown(fd_, SD_BOTH);
	::shutdown(fd_, SHUT_RDWR);
	evutil_closesocket(fd_);
	fd_ = -1;
	state_ = CNT_STATE_DISCONNECTED;	
}