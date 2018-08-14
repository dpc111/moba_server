#include "stdafx.h"
#include "player_mgr.h"
#include "player.h"

player_mgr_t::player_mgr_t() {
	players_.clear();
	player_num_ = 0;
}

player_mgr_t::~player_mgr_t() {
	while (!players_.empty()) {
		player_map_t::iterator it = players_.begin();
		player_t *player = it->second;
		delete player;
		players_.erase(it);
		--player_num_;
	}
}

player_t* player_mgr_t::get_player(int64 uid) {
	player_map_t::iterator it = players_.find(uid);
	if (it == players_.end()) {
		return NULL;
	}
	return it->second;
}

void player_mgr_t::add_player(int64 uid, player_t *player) {
	if (this->get_player(uid)) {
		return;
	}
	players_[uid] = player;
	++player_num_;
}

void player_mgr_t::del_player(int64 uid) {
	player_map_t::iterator it = players_.find(uid);
	if (it == players_.end()) {
		return;
	}
	player_t *player = it->second;
	players_.erase(it);
	--player_num_;
	delete player;
}

player_t* player_mgr_t::create_player(int64 uid) {
	if (this->get_player(uid)) {
		return NULL;
	}
	player_t *player = new player_t(uid, "...", 1, ".....");
	this->add_player(uid, player);
	return player;
}

void player_mgr_t::kick(int64 uid) {
	del_player(uid);
}