#ifndef TIMER_H
#define TIMER_H

#include <assert.h>
#include <stdio.h>
#include <algorithm>
#include <tr1/functional>
#include "type.h"
#include "timer_axis.h"

class timer_mt {
public:
	typedef std::tr1::function<void (void *)> cb_t;

public:
	timer_mt(const cb_t& cb, void *obj, void *data, const char *name, uint32 times, uint32 interval);

	~timer_mt();

	void on_register();

	void on_unregister();

	cb_t cb_;

	void *obj_;

	void *data_;

	const char *name_;

	uint32 times_;

	uint32 interval_;

	uint32 slot_;

	timer_axis_t::slot_t::iterator pos_;

	timestamp last_time_;
};

#endif