#include "timer_handle.h"
#include "timer_axis.h"
#include <math.h>
#include <string>

timer_handle_t::timer_handle_t(timer_axis_t *timer_axis) {
	timers_.clear();
	name_timers_.clear();
	timer_axis_ = timer_axis;
}

timer_handle_t::~timer_handle_t() {
	timers_t::iterator it = timers_.begin();
	while (it != timers_.end()) {
		timer_mt *timer = it->second;
		timer_axis_->unregister(timer);
		it = timers_.begin();
	}
	timers_.clear();
	name_timers_.clear();
	timer_axis_ = NULL;
}

void timer_handle_t::register_timer_ms(const timer_mt::cb_t& cb, uint32 interval, uint32 times, const char *name, void *data) {
	timer_mt *timer = new timer_mt(cb, this, data, name, times, interval);
	timer_axis_->registertimer(timer);
}

void timer_handle_t::register_timer_sec(const timer_mt::cb_t& cb, float interval, uint32 times, const char *name, void *data) {
	register_timer_ms(cb,
		floor(interval * 1000),
		times,
		name,
		data);
}

void timer_handle_t::register_timer_repeat(const timer_mt::cb_t& cb, float interval, const char *name, void *data) {
	register_timer_ms(cb,
		floor(interval * 1000),
		~0u,
		name,
		data);
}

void timer_handle_t::register_timer_delay(const timer_mt::cb_t& cb, float interval, const char *name, void *data) {
	register_timer_ms(cb,
		floor(interval * 1000),
		1,
		name,
		data);
}

void timer_handle_t::unregister_timer(const char *pname) {
	std::string name(pname);
	name_timers_t::iterator it = name_timers_.find(name);
	if (it == name_timers_.end()) {
		return;
	}
	timer_mt *timer = it->second;
	timer_axis_->unregister(timer);
}

void timer_handle_t::on_register(timer_mt *timer) {
	int64 pos = int64(timer);
	timers_t::iterator it = timers_.find(pos);
	if (it != timers_.end()) {
		return;
	}
	timers_[pos] = timer;
	if (timer->name_) {
		std::string name(timer->name_);
		name_timers_t::iterator it = name_timers_.find(name);
		if (it != name_timers_.end()) {
			return;
		}
		name_timers_[name] = timer;
	}
}

void timer_handle_t::on_unregister(timer_mt *timer) {
	int64 pos = int64(timer);
	timers_t::iterator it = timers_.find(pos);
	if (it == timers_.end()) {
		return;
	}
	timers_.erase(it);
	if (timer->name_) {
		std::string name(timer->name_);
		name_timers_t::iterator it = name_timers_.find(name);
		if (it == name_timers_.end()) {
			return;
		}
		name_timers_.erase(it);
	}
}