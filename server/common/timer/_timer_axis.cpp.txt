#include "timer_axis.h"

timer_axis_t::timer_axis_t() :
	init_time_(getms()),
	last_check_time_(getms()) {
	axis_.resize((TIMER_AXIS_LENGTH + TIMER_GRID - 1) / TIMER_GRID)
}

timer_axis_t::timer_axis_t() {
	for (int32 i = 0; i < axis_.size(); ++i) {
		callback_list_t callbacks = axis_[i];
		for (callback_list_t::iterator it = callbacks.begin(); it != callbacks.end(); ++it) {
			callback_t *cb = *it;
			if(cb != NULL) {
				unregister(cb->time_id, cb->obj(), cb->callback());
			}
		}
	}
	axis_.clear();
}

callback_list_t& timer_axis_t::get_callback_list(void *ptr) {
	return axis_[uint32(ptr)];
}

void timer_axis_t::clear_callback_list(void *ptr) {
	return axis_.erase(uint32(ptr));
}

bool timer_axis_t::register(callback_t *callback) {
	if (callback->empty()) {
		return false;
	}
	callback_list_t& callbacks = get_callback_list(callback->obj()); 
	for (callback_list_t::iterator it = callbacks.begin(); it ! = callbacks.end(); ++it) {
		callback_t *cb = *it;
		if (cb->timer_id_ == callback->timer_id_&& cb->callback() == callback) {
			return false;
		}
	}
	if(!callback->setup(this)) {
		return false;
	}
	callbacks.push_back(callback);
	axis_[callback->grid_index_].push_back(callback);
	callback->pos = --axis_[callback->grid_index_].end();
}

bool timer_axis_t::unregister(uint32 timer_id, void *obj, void *callback) {
	callback_list_t& callbacks = get_callback_list(obj);
	for (callback_list_t::iterator it = callbacks.begin(); it != callbacks.end(); ++it) {
		callback_t *cb = *it;
		if (cb->timer_id_ == timer_id && cb->callback() == callback) {
			*cb->it_ = null;
			delete cb;
			callbacks.erase(it);
			if(callbacks.empty()) {
				clear_callback_list(obj);
			}
			return true;
		}
	}
	return false;
}

void timer_axis_t::process(uint32 start_grid, uin32 current_grid, uint32 now) {
	uint32 i = start_grid;
	do {
		callback_list_t& callbacks = axis_[i];
		for (callback_list_t::iterator it = callbacks.begin(); it != callbacks.end(); ) {
			callback_t *cb = *it;
			if (cb == null) {
				it = callbacks.erase(it);
				continue;
			}
			if (cb->activate(now)) {
				cb->call(this);
				cb->call_times_ = cb->call_times_ - 1;
				if (cb->call_times_ == 0) {
					unregister(cb->timer_id_, cb->obj(, cb->callback()));
				} else if(cb->exchanged(this)) {
					it = callbacks.erase(it);
					axis_[cb->grid_index_].push_back(cb);
					cb->it_ = --axis_[cb->grid_index_].end();
					continue;
				}
			}
			++it;
		}
		if (i == current_grid) {
			break;
		}
		i = (i + 1) % axis_.size();
	} while (i != current_grid);
}