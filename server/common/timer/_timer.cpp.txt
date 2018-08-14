#include "timer.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
void timer_handle_t::cancel() {
	if (timer_ != NULL) {
		timer_->cancel();
		timer_ = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
ctimer_t::ctimer_t(timers_t &owner, timer_handler_t *handler, void *user_data, timestamp time, timestamp interval)
	: owner_(owner)
	, handler_(handler)
	, user_data_(user_data)
	, time_(time)
	, interval_(interval)
	, state_(TIME_PENDING) {
	handler_->inc_register_num();
}

void ctimer_t::cancel() {
	if (this->is_cancelled()) {
		return;
	}
	assert((state_ == TIME_PENDING) || (state_ == TIME_EXECUTING));
	state_ = TIME_CANCELLED;
	if (handler_) {
		handler_->release(timer_handle_t(this), user_data_);
		handler_ = NULL;
	}
	owner_.on_cancel(); 
}

void ctimer_t::trigger(timestamp now) {
	if (!this->is_cancelled()) {
		state_ = TIME_EXECUTING;
		handler_->handle_timeout(timer_handle_t(this), user_data_);
		if ((interval_ == 0) && !this->is_cancelled()) {
			this->cancel();
		}
	}
	if (!this->is_cancelled()) {
		// time_ += interval_;
		time_ = now + interval_;
		state_ = TIME_PENDING;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
timers_t::timers_t()
	: time_queue_()
	, process_node_(NULL)
	, last_process_time_(0)
	, cancelled_num_(0) {
}

timers_t::~timers_t() {
	this->clear();
}

int timers_t::process(timestamp now) {
	int fired_num = 0;
	while ((!time_queue_.empty()) &&
		(time_queue_.top()->time() <= now ||
		time_queue_.top()->is_cancelled())) {
		ctimer_t *timer = process_node_ = time_queue_.top();
		time_queue_.pop();
		if (!timer->is_cancelled()) {
			++fired_num;
			timer->trigger(now);
		}
		if (!timer->is_cancelled()) {
			time_queue_.push(timer);
		} else {
			delete timer;
			assert(cancelled_num_ > 0);
			--cancelled_num_;
		}
	}
	process_node_ = NULL;
	last_process_time_ = now;
	return fired_num;
}

void timers_t::clear(bool should_call_cancel) {
	int max_loop_num = time_queue_.size();
	while (!time_queue_.empty()) {
		ctimer_t *timer = time_queue_.unsafe_pop_back();
		if (!timer->is_cancelled() && should_call_cancel) {
			--cancelled_num_;
			timer->cancel();
			if (--max_loop_num == 0) {
				should_call_cancel = false;
			}
		} else if (timer->is_cancelled()) {
			--cancelled_num_;
		}
		delete timer;
	}
	cancelled_num_ = 0;
	time_queue_ = priority_queue_t();
}

bool timers_t::get_timer_info(timer_handle_t handle, void *& user, timestamp& time, timestamp& interval) const {
	ctimer_t *timer = handle.timer();
	if (!timer->is_cancelled()) {
		user = timer->user_data();
		time = timer->time();
		interval = timer->interval();
		return true;
	}
	return false;
}

timer_handle_t timers_t::add(timer_handler_t *handler, void *user, timestamp time, timestamp interval) {
	ctimer_t *timer = new ctimer_t(*this, handler, user, time, interval);
	time_queue_.push(timer);
	return timer_handle_t(timer);
}

void timers_t::purge_cancelled_times() {
	priority_queue_t::container_t& container = time_queue_.container();
	priority_queue_t::container_t::iterator new_end = std::partition(container.begin(), container.end(), timers_t::is_not_cancelled);
	for (priority_queue_t::container_t::iterator it = new_end; it != container.end(); ++it) {
		delete *it;
	} 
	const int purged_num = container.end() - new_end;
	cancelled_num_ -= purged_num;
	assert((cancelled_num_ == 0) || (cancelled_num_ == 1));
	container.erase(new_end, container.end());
	time_queue_.make_heap();
}

void timers_t::on_cancel() {
	++cancelled_num_;
	if ((cancelled_num_ * 2) > int(time_queue_.size())) {
		this->purge_cancelled_times();
	}
}
