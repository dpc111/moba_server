#ifndef TIMER_AXIS_H
#define TIMER_AXIS_H

#include <list>
#include <vector>
#include "type.h"

#define TIMER_SLOT_LENGTH 60
#define TIMER_AXIS_LENGTH 600000

class timer_mt;

class timer_axis_t {
public:
	typedef std::list<timer_mt *> slot_t;

	typedef std::vector<slot_t> axis_t;

public:
	timer_axis_t();

	~timer_axis_t();

	timestamp get_init_time() const { return init_time_; }

	timestamp get_last_time() const { return last_time_; }

	void registertimer(timer_mt *timer);

	void unregister(timer_mt* timer);

	void process();

private:
	axis_t axis_;

	uint32 axis_size_;

	timestamp init_time_;

	timestamp last_time_;
};

#endif