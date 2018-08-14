#ifndef STATE_CACHE_H
#define STATE_CACHE_H

#include "stdafx.h"
#include "message/messages.h"

class room_t;

class state_cache_t {
public:
	state_cache_t(room_t *room);

	~state_cache_t();

	void add_cache(int32 state, int32 id);

	void add_cache(int32 state, int32 id, int32 p1, int32 p2, int32 p3);

	void sync();

	void update(double tm);

private:
	room_t *room_;

	double last_sync_time_;

	battle_msg::s_update_state cache_;
};

#endif