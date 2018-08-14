#include "global.h"
#include "player_mgr.h"
#include "room_mgr.h"
#include "service.h"
#include "cfg_json_mgr.h"

service_t *g_service_ = new service_t();

player_mgr_t* get_player_mgr() {
	return get_service()->get_player_mgr();
}

room_mgr_t* get_room_mgr() {
	return get_service()->get_room_mgr();
}

cfg_json_mgr_t* get_json_mgr() {
	return get_service()->get_json_mgr();
}

service_t* get_service() {
	return g_service_;
}
