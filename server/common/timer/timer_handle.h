#ifndef TIMER_HANDLE_H
#define TIMER_HANDLE_H

#include "timer.h"
#include <tr1/functional>
#include <map>

#define TIMER_CALLBACK(func) std::tr1::bind(&func, this, std::tr1::placeholders::_1)

class timer_handle_t {
public:
	typedef void (callback_t) (void *);

	typedef std::map<int64, timer_mt *> timers_t;

	typedef std::map<std::string, timer_mt *> name_timers_t;

public:
	timer_handle_t(timer_axis_t *timer_axis);

	virtual ~timer_handle_t();

	virtual void register_timer_ms(const timer_mt::cb_t& cb, uint32 interval, uint32 times = 1, const char *name = NULL, void *data = NULL);

	virtual void register_timer_sec(const timer_mt::cb_t& cb, float interval, uint32 times = 1, const char *name = NULL, void *data = NULL);

	virtual void register_timer_repeat(const timer_mt::cb_t& cb, float interval, const char *name = NULL, void *data = NULL);

	virtual void register_timer_delay(const timer_mt::cb_t& cb, float interval, const char *name = NULL, void *data = NULL);

	virtual void unregister_timer(const char *name);

	void on_register(timer_mt *timer);

	void on_unregister(timer_mt *timer);

protected:
	timers_t timers_;

	name_timers_t name_timers_;

	timer_axis_t *timer_axis_;
};

#endif