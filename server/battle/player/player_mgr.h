#ifndef PLAYER_MGR_H
#define PLAYER_MGR_H

#include "stdafx.h"
#include <map>

class player_t;

class player_mgr_t {
public:
	typedef std::map<int64, player_t*> player_map_t;

public:
	player_mgr_t();

	~player_mgr_t();

	player_t* get_player(int64 uid);

	void add_player(int64 uid, player_t *player);

	void del_player(int64 uid);

	player_t* create_player(int64 uid);

	void kick(int64 uid);

	int32 player_num() { return player_num_; }

private:
	player_map_t players_;

	int32 player_num_;
};

#endif