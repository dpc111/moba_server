#include "stdafx.h"
#include "room_mgr.h"
#include "room.h"
#include "id_generate.h"

room_mgr_t::room_mgr_t() :
	id_generate_() {

}

room_mgr_t::~room_mgr_t() {

}

room_t* room_mgr_t::get_room(int rid) {
	room_map_t::iterator it = rooms_.find(rid);
	if (it == rooms_.end()) {
		return NULL;
	}
	return it->second;
}

void room_mgr_t::add_room(int rid, room_t *room) {
	if (this->get_room(rid)) {
		return;
	}
	rooms_[rid] = room;
	++room_num_;
}

void room_mgr_t::del_room(int rid) {
	ERROR("del room %d", rid);
	room_map_t::iterator it = rooms_.find(rid);
	if (it == rooms_.end()) {
		ERROR("");
		return;
	}
	room_t *room = it->second;
	rooms_.erase(it);
	--room_num_;
	delete room;
}

room_t* room_mgr_t::create_room() {
	int32 rid = id_generate_.gen();
	if (get_room(rid)) {
		return NULL;
	}
	room_t *room = new room_t(rid);
	add_room(rid, room);
	room->start_wait();
	return room;
}

room_t* room_mgr_t::get_idle_room(bool bcreate) {
	for (room_map_t::iterator it = rooms_.begin(); it != rooms_.end(); ) {
		room_t *room = it->second;
		if (room->get_player_num() < 2) {
			return room;
		}
	}
	if (bcreate) {
		return this->create_room();
	}
	return NULL;
}

void room_mgr_t::update(int64 tm) {
	for (room_map_t::iterator it = rooms_.begin(); it != rooms_.end(); ) {
		room_t *room = it->second;
		if (room->get_del()) {
			++it;
			del_room(room->get_rid());
			continue;
		}
		room->update(tm);
		++it;
	}
}