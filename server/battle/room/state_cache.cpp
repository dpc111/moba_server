#include "state_cache.h"
#include "room.h"

state_cache_t::state_cache_t(room_t *room) {
	room_ = room;
	last_sync_time_ = getfs();
	cache_.mutable_states()->Clear();
}

state_cache_t::~state_cache_t() {
	room_ = NULL;
	last_sync_time_ = 0;
	cache_.mutable_states()->Clear();
}

void state_cache_t::add_cache(int32 state, int32 id) {
	LOG("%d  %d", state, id);
	battle_msg::update_state *st = cache_.add_states();
	st->set_state(state);
	st->set_id(id);
}

void state_cache_t::add_cache(int32 state, int32 id, int32 p1, int32 p2, int32 p3) {
	battle_msg::update_state *st = cache_.add_states();
	st->set_state(state);
	st->set_id(id);
	st->set_p1(p1);
	st->set_p2(p2);
	st->set_p3(p3);
}

void state_cache_t::sync() {
	room_->broadcast(cache_);
	cache_.mutable_states()->Clear();
	last_sync_time_ = getfs();
}

void state_cache_t::update(double tm) {
	if (tm - last_sync_time_ > 0.05) {
		if (cache_.states_size() > 0) {
			sync();
		}
	}
}