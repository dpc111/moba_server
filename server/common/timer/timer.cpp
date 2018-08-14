#include "timer.h"
#include "timer_handle.h"

timer_mt::timer_mt(const cb_t& cb, void *obj, void *data, const char *name, uint32 times, uint32 interval) :
	cb_(cb),
	obj_(obj),
	data_(data),
	name_(name),
	times_(times),
	interval_(interval),
	slot_(0),
	last_time_(0) { 

}

timer_mt::~timer_mt() {
	cb_ = NULL;
	obj_ = NULL;
	data_ = 0;
	name_ = NULL;
	times_ = 0;
	interval_ = 0;
	slot_ = 0;
	last_time_ = 0;
}

void timer_mt::on_register() {
	timer_handle_t *handle = static_cast<timer_handle_t *> (obj_);
	handle->on_register(this);
}

void timer_mt::on_unregister() {
	timer_handle_t *handle = static_cast<timer_handle_t *> (obj_);
	handle->on_unregister(this);
}