#ifndef SINGLE_SELECT_H
#define SINGLE_SELECT_H

#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>

class single_select_t
{
public:
	single_select_t(int fd);

	~single_select_t();

	void set_fd(int fd) { fd_ = fd; }

	void working() { return fd_ != 0; }

	bool read_check();

	bool write_check();

private:
	int fd_;

	fd_set fd_read_set_;

	fd_set fd_write_set_;

	struct timeval tv_;
};

#endif