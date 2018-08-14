#include "client_msg.h"
#include "service.h"
#include "global.h"
#include "player_mgr.h"
#include "player.h"
#include "room_mgr.h"
#include "room.h"

client_msg_t::client_msg_t() {

}

client_msg_t::~client_msg_t() {
	
}

void client_msg_t::c_login_hall(tcp_connection_t *conn, const battle_msg::c_login_hall& msg) {
	battle_msg::s_login_hall res;
	res.set_res("ok");
	res.set_uid(msg.uid());
	res.set_name("name");
	res.set_icon("icon");
	get_service()->send(conn, res);
}

void client_msg_t::c_login(tcp_connection_t *conn, const battle_msg::c_login& msg) {
	battle_msg::s_login res;
	int64 uid = msg.uid();
	player_t *player = get_player_mgr()->get_player(uid);
	if (player) {
		res.set_res("ok");
		res.set_uid(player->get_uid());
		res.set_name(player->get_name());
		res.set_icon(player->get_icon());
		res.set_camp(player->get_camp());
		get_service()->send(conn, res);
		get_service()->verify((int32)uid, conn);
		return;
	}
	player = get_player_mgr()->create_player(uid);
	if (!player) {
		res.set_res("fail");
		get_service()->send(conn, res);
		return;
	}
	room_t *room = get_room_mgr()->get_idle_room(true);
	if (!room) {
		ERROR("");
		return;
	}
	player->set_room(room);
	int32 camp = room->set_random_camp(player->get_uid());
	player->set_camp(camp);
	res.set_res("ok");
	res.set_uid(player->get_uid());
	res.set_name(player->get_name());
	res.set_icon(player->get_icon());
	res.set_camp(player->get_camp());
	get_service()->send(conn, res);
	get_service()->verify((int32)uid, conn);
}

void client_msg_t::default_message(int32 uid, const google::protobuf::Descriptor* des, const google::protobuf::Message *msg) {
	player_t *player = get_player_mgr()->get_player(uid);
	if (!player) {
		ERROR("");
		return;
	}
	room_t *room = player->get_room();
	if (!room) {
		ERROR("");
		return;
	}
	room->on_message(player, des, msg);
}