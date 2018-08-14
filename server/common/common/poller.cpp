#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "assert.h"
#include "poller.h"

poller_t::poller_t() {
	epoll_fd_ = epoll_create(POLLER_MAX_SIZE);
	if (epoll_fd_ == -1) {
		//error
		return;
	}
}

poller_t::~poller_t() {
	close(epoll_fd_);
}

void poller_t::register_read(int fd, void *ud, read_func *func) {
	read_handle_t handle;
	handle.ud_ = ud;
	handle.func_ = func;
	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.data.fd = fd;
	int op;
	bool bread = is_register_read(fd);
	bool bwrite = is_register_write(fd);
	if (bread || bwrite) {
		op = EPOLL_CTL_MOD;
		ev.events = bwrite ? EPOLLIN | EPOLLOUT : EPOLLIN;
	} else {
		op = EPOLL_CTL_ADD;
		ev.events = EPOLLIN;
	}
	if (epoll_ctl(epoll_fd_, op, fd, &ev) < 0) {
		return;
	}
	read_handles_[fd] = handle;
}

void poller_t::register_write(int fd, void *ud, write_func *func) {
	write_handle_t handle;
	handle.ud_ = ud;
	handle.func_ = func;
	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.data.fd = fd;
	int op;
	bool bread = is_register_read(fd);
	bool bwrite = is_register_write(fd);
	if (bread || bwrite) {
		op = EPOLL_CTL_MOD;
		ev.events = bread ? EPOLLIN | EPOLLOUT : EPOLLOUT;
	} else {
		op = EPOLL_CTL_ADD;
		ev.events = EPOLLOUT;
	}
	if (epoll_ctl(epoll_fd_, op, fd, &ev) < 0) {
		return;
	}
	write_handles_[fd] = handle;
}

void poller_t::register_listen(int fd, void *ud, listen_func *func) {
	listen_handle_t handle;
	handle.ud_ = ud;
	handle.func_ = func;
	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.data.fd = fd;
	ev.events = EPOLLIN|EPOLLET;
	bool bread = is_register_read(fd);
	bool bwrite = is_register_write(fd);
	assert(!bread);
	assert(!bwrite);
	if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev) < 0) {
		return;
	}
	listen_handles_[fd] = handle;
}

void poller_t::deregister_read(int fd) {
	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.data.fd = fd;
	int op;
	bool bwrite = is_register_write(fd);
	if (!bwrite) {
		op = EPOLL_CTL_DEL;
	} else {
		op = EPOLL_CTL_MOD;
		ev.events = EPOLLOUT;
	}
	if (epoll_ctl(epoll_fd_, op, fd, &ev) < 0) {
		return;
	}
	read_handles_.erase(fd);
}

void poller_t::deregister_write(int fd) {
	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.data.fd = fd;
	int op;
	bool bread = is_register_read(fd);
	if (!bread) {
		op = EPOLL_CTL_DEL;
	}
	else {
		op = EPOLL_CTL_MOD;
		ev.events = EPOLLIN;
	}
	if (epoll_ctl(epoll_fd_, op, fd, &ev) < 0) {
		return;
	}
	write_handles_.erase(fd);
}

void poller_t::deregister_listen(int fd) {
	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.data.fd = fd;
	if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, &ev) < 0) {
		return;
	}
	listen_handles_.erase(fd);
}

void poller_t::deregister_all(int fd) {
	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.data.fd = fd;
	int op = EPOLL_CTL_DEL;
	read_handles_.erase(fd);
	write_handles_.erase(fd);
	listen_handles_.erase(fd);
	if (epoll_ctl(epoll_fd_, op, fd, &ev) < 0) {
		return;
	}
}

int poller_t::process_once(int wait_time) {
	memset(events_, 0, sizeof(epoll_event) * POLLER_EVENTS_ONCE);
	int nfds = epoll_wait(epoll_fd_, events_, POLLER_EVENTS_ONCE, wait_time);
	for (int i = 0; i < nfds; i++) {		
		if (events_[i].events & (EPOLLERR | EPOLLHUP)) {
			trigger_error(events_[i].data.fd);
		} 
		else {
			listen_handles_t::iterator it = listen_handles_.find(events_[i].data.fd);
			if (it != listen_handles_.end()) {
				trigger_listen(events_[i].data.fd);
				return 0;
			}
			if (events_[i].events & EPOLLIN) {
				trigger_read(events_[i].data.fd);
			}
			if (events_[i].events & EPOLLOUT) {
				trigger_write(events_[i].data.fd);
			}
		}
	}
	return nfds;
}

bool poller_t::is_register_read(int fd) {
	return read_handles_.find(fd) != read_handles_.end();
}

bool poller_t::is_register_write(int fd) {
	return write_handles_.find(fd) != write_handles_.end();
}

bool poller_t::trigger_read(int fd) {
	read_handles_t::iterator iter = read_handles_.find(fd);
	if (iter == read_handles_.end()) {
		return false;
	}
	read_handle_t& handle = iter->second;
	handle.func_(fd, handle.ud_);
	return true;
}

bool poller_t::trigger_write(int fd) {
	write_handles_t::iterator iter = write_handles_.find(fd);
	if (iter == write_handles_.end()) {
		return false;
	}
	write_handle_t& handle = iter->second;
	handle.func_(fd, handle.ud_);
	return true;
}

bool poller_t::trigger_listen(int fd) {
	listen_handles_t::iterator iter = listen_handles_.find(fd);
	if (iter == listen_handles_.end()) {
		return false;
	}
	listen_handle_t& handle = iter->second;
	handle.func_(fd, handle.ud_);
	return true;
}

bool poller_t::trigger_error(int fd) {
	if (!trigger_read(fd)) {
		return trigger_write(fd);
	}
	return true;
}
