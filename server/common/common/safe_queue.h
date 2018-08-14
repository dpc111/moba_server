#ifndef CMD_QUEUE_H
#define CMD_QUEUE_H

#include <queue>
#include "thread.h"

template <typename _t>
class safe_queue_t
{
public:
	safe_queue_t() {
		size_ = 0;
	}

	~safe_queue_t() {
		lock_.lock();
		while (!buff_.empty()) {
			buff_.pop();
		}
		lock_.unlock();
		size_ = 0;
	}

	void push(_t t) {
		lock_.lock();
		buff_.push(t);
		size_++;
		lock_.unlock();
	}

	_t pop() {
		lock_.lock();
		if (size_ <= 0) {
			return NULL;
		}
		_t t = buff_.front();
		buff_.pop();
		size_--;
		lock_.unlock();
		return t;
	}

	int size() {
		return size_;
	}
	
private:
	lock_t lock_;
	std::queue<_t> buff_;
	int size_;
};

#endif