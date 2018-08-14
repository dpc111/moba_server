#include "single_select.h"

single_select_t::single_select_t(int fd) {
	fd_ = fd;
	FD_ZERO(&fd_read_set_);
	FD_ZERO(&fd_write_set_);
	tv_.tv_sec = 0;
    tv_.tv_usec = 0;
}

single_select_t::~single_select_t() {
	fd_ = 0;
	FD_ZERO(&fd_read_set_);
	FD_ZERO(&fd_write_set_);
}

bool single_select_t::read_check() {
	FD_ZERO(&fd_read_set_);
	FD_SET(fd_, &fd_read_set_);
	if (select(1, &fd_read_set_, NULL, NULL, &tv_) < 0) {
		return false;
	}
	if (FD_ISSET(fd_, &fd_read_set_)) {
		return true;
	}
	return false;
}

bool single_select_t::write_check() {
	FD_ZERO(&fd_wrire_set_);
	FD_SET(fd_, &fd_wrire_set_);
	if (select(1, &fd_wrire_set_, NULL, NULL, &tv_) < 0) {
		return false;
	}
	if (FD_ISSET(fd_, &fd_wrire_set_)) {
		return true;
	}
	return false;
}