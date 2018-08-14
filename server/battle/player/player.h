#ifndef PLAYER_H
#define PLAYER_H

#include "stdafx.h"
#include "room.h"

class player_t {
public:
	player_t(int64 uid, std::string name, int32 level, std::string icon);

	~player_t();

	int64 get_uid() { return uid_; }

	const char* get_name() { return name_.c_str(); }

	int32 get_level() { return level_; }

	const char* get_icon() { return icon_.c_str(); }

	room_t* get_room() { return room_; }

	void set_room(room_t *room) { room_ = room; }

	int32 get_camp() { return camp_; }

	void set_camp(int32 camp) { camp_ = camp; }

private:
	int64 uid_;

	std::string name_;

	int32 level_;

	std::string icon_;

	bool is_online_;

	room_t *room_;

	int32 camp_;
};

#endif