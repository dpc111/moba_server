#ifndef TIMER_H
#define TIMER_H

#include <assert.h>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include "type.h"

class ctimer_t;
class timers_t;

//////////////////////////////////////////////////////////////////////////////////////////////////
class timer_handle_t {
public:
	timer_handle_t(ctimer_t *timer = NULL) : timer_(timer) {}

	void cancel();

	void clear() { timer_ = NULL; }

	bool is_set() const { return timer_ != NULL; }

	friend bool operator==(timer_handle_t h1, timer_handle_t h2);

	timer_handle_t& operator=(const timer_handle_t& h) { timer_ = h.timer(); return *this; }

	ctimer_t *timer() const { return timer_; }

private:
	ctimer_t *timer_;
};

inline bool operator==(timer_handle_t h1, timer_handle_t h2) {
	return h1.timer_ == h2.timer_;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
class timer_handler_t {
public:
	timer_handler_t() : register_num_(0) {}

	virtual ~timer_handler_t() { assert(register_num_ == 0); }

	virtual void handle_timeout(timer_handle_t handle, void *user) = 0;

protected:
	virtual void on_release(timer_handle_t handle, void *user) {}

private:
	friend class ctimer_t;

	void inc_register_num() { ++register_num_; }

	void dec_register_num() { --register_num_; }

	void release(timer_handle_t handle, void *user) {
		this->dec_register_num();
		this->on_release(handle, user);
	}

private:
	int register_num_;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
class ctimer_t {
public:
	enum time_state_t {
		TIME_PENDING,
		TIME_EXECUTING,
		TIME_CANCELLED
	};

public:
	ctimer_t(timers_t &owner, timer_handler_t *handler, void *user_data, timestamp time, timestamp interval);
	
	ctimer_t(const ctimer_t &);

	~ctimer_t() {}

	ctimer_t &operator=(const ctimer_t &);

	void cancel();

	void trigger(timestamp now);

	timers_t& owner() const { return owner_; }

	timer_handler_t *handler() const { return handler_; }

	void *user_data() const { return user_data_; }

	timestamp time() const { return time_; }

	timestamp interval() const { return interval_; }

	bool is_cancelled() const { return state_ == TIME_CANCELLED; }

	bool is_executing() const { return state_ == TIME_EXECUTING; }

private:
	timers_t& owner_;

	timer_handler_t *handler_;

	void *user_data_;

	timestamp time_;

	timestamp interval_;

	time_state_t state_;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
class priority_queue_t {
public:
	typedef std::vector<ctimer_t *> container_t;

public:
	static bool compare(const ctimer_t *a, const ctimer_t *b) { return a->time() > b->time(); }

	bool empty() const { return container_.empty(); }

	int size() const { return container_.size(); }

	const container_t::value_type & top() const { return container_.front(); }

	void push(const container_t::value_type & x) {
		container_.push_back(x);
		std::push_heap(container_.begin(), container_.end(), priority_queue_t::compare);
	}

	void pop() {
		std::pop_heap(container_.begin(), container_.end(), priority_queue_t::compare);
		container_.pop_back();
	}

	ctimer_t *unsafe_pop_back() {
		ctimer_t *time = container_.back();
		container_.pop_back();
		return time;
	}

	container_t &container() { return container_; }

	void make_heap() { std::make_heap(container_.begin(), container_.end(), priority_queue_t::compare); }

private:
	container_t container_;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
class timers_t {
public:
	timers_t();

	virtual ~timers_t();

	static bool is_not_cancelled(const ctimer_t *timer) { return !timer->is_cancelled(); }

	inline int size() const { return time_queue_.size(); }

	inline bool empty() const { return time_queue_.empty(); }

	int process(timestamp now);

	void clear(bool should_call_cancel = true);

	bool get_timer_info(timer_handle_t handle, void *& user, timestamp& time, timestamp& interval) const;

	timer_handle_t add(timer_handler_t *handler, void *user, timestamp start, timestamp interval);

	void purge_cancelled_times();

	void on_cancel();

private:
	priority_queue_t time_queue_;

	ctimer_t *process_node_;

	timestamp last_process_time_;

	int cancelled_num_;
};

#endif