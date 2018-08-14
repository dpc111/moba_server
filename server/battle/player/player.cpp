#include "player.h"

player_t::player_t(int64 uid, std::string name, int32 level, std::string icon) {
	uid_ = uid;
	name_ = name;
	level_ = level;
	icon_ = icon;
	is_online_ = false;
	camp_ = 0;
}

player_t::~player_t() {
	
}