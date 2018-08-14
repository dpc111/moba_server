#include "timer_axis.h"
#include "timer.h"
#include "timestamp.h"

timer_axis_t::timer_axis_t() {
	assert(TIMER_AXIS_LENGTH % TIMER_SLOT_LENGTH == 0);
	axis_size_ = TIMER_AXIS_LENGTH / TIMER_SLOT_LENGTH;
	axis_.resize(axis_size_);
	init_time_ = getms();
	last_time_ = getms();
}

timer_axis_t::~timer_axis_t() {
	for (axis_t::iterator it = axis_.begin(); it != axis_.end(); ++it) {
		slot_t& slot = *it;
		for (slot_t::iterator it1 = slot.begin(); it1 != slot.end(); ++it1) {
			timer_mt *timer = *it1;
			if (timer != NULL) {
				unregister(timer);
			}
		}
		slot.clear();
	}
	axis_.clear();
}

void timer_axis_t::registertimer(timer_mt* timer) {
	if (timer->interval_ == 0) {
		timer->interval_ = 1;
	}
	timer->last_time_ = last_time_;
	timer->slot_ = ((timer->last_time_ + timer->interval_ - init_time_) / TIMER_SLOT_LENGTH) % axis_size_;
	slot_t& slot = axis_[timer->slot_];
	slot.push_back(timer);
	timer->pos_ = --slot.end();
	timer->on_register();
}

void timer_axis_t::unregister(timer_mt* timer) {
	// 不直接删除 防止定时器主循环崩溃
	timer->on_unregister();
	*(timer->pos_) = NULL;
	delete timer;
}

void timer_axis_t::process() {
	timestamp now = getms();
	uint32 start = ((last_time_ - init_time_) / TIMER_SLOT_LENGTH) % axis_size_;
	uint32 end = ((now - init_time_) / TIMER_SLOT_LENGTH) % axis_size_;
	uint32 cur = start;
	do {
		slot_t& slot = axis_[cur];
		for (slot_t::iterator it = slot.begin(); it != slot.end(); ) {
			timer_mt *timer = *it;
			if (timer == NULL) {
				it = slot.erase(it);
				continue;
			}
			if (now - timer->last_time_ >= timer->interval_) {
				timer->cb_(timer->data_);
				timer->last_time_ = now;
				--timer->times_;
				if (timer->times_ <= 0) {
					unregister(timer);
					continue;
				} else {
					uint32 newslot = ((timer->last_time_ + timer->interval_ - init_time_) / TIMER_SLOT_LENGTH) % axis_size_;
					if (newslot != timer->slot_) {
						it = slot.erase(timer->pos_);
						axis_[newslot].push_back(timer);
						timer->pos_ = --axis_[newslot].end();
						timer->slot_ = newslot;
						continue;
					}
				}
			}
			++it;
		}
		if (cur == end) {
			break;
		}
		cur = (cur + 1) % axis_size_;
	} while (cur != end);
	last_time_ = now;
}