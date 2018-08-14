#include "room.h"
#include "entity_mgr.h"
#include "bullet_mgr.h"
#include "entity.h"
#include "bullet.h"
#include "grid.h"
#include "player.h"
#include "service.h"
#include "global.h"
#include "player_mgr.h"
#include "state_cache.h"

#define REGISTER_ROOM_BIND(func) \
	std::tr1::bind(&func, \
	this, \
	std::tr1::placeholders::_1, \
	std::tr1::placeholders::_2)

room_t::room_t(int32 rid) :
	dispatcher_t(),
	timer_handle_t(get_service()->get_timer_axis()) {
	rid_ = rid;
	entity_mgr_ = new entity_mgr_t(this);
	bullet_mgr_ = new bullet_mgr_t(this);
	grid_ = new grid_t(this);
	state_cache_ = new state_cache_t(this);
	del_ = false;
	room_state_ = ROOM_STATE_WAIT;
	begin_time_ = getfs();
	player_num_ = 0;
	for (int32 k = 0; k < 2; k++) {
		camps_[k] = 0;
	}
	register_callback();
}

room_t::~room_t() {
	delete entity_mgr_;
	entity_mgr_ = NULL;
	delete bullet_mgr_;
	bullet_mgr_ = NULL;
	delete grid_;
	grid_ = NULL;
	delete state_cache_;
	state_cache_ = NULL;
	for (int32 k = 0; k < 2; k++) {
		camps_[k] = 0;
	}
}

int32 room_t::set_random_camp(int64 uid) {
	for (int32 k = 0; k < 2; k++) {
		if (camps_[k] == 0) {
			camps_[k] = uid;
			player_num_++;
			return k + 1;
		}
	}
	return 0;
}

void room_t::update(int64 tm) {
	if (del_ || room_state_ != ROOM_STATE_ING) {
		return;
	}
	double stm = (double)tm / 1000;
	entity_mgr_->update(stm);
	bullet_mgr_->update(stm);
	state_cache_->update(stm);
}

void room_t::register_callback() {
	register_message<battle_msg::c_get_room_info>(REGISTER_ROOM_BIND(room_t::c_get_room_info));
	register_message<battle_msg::c_create_entity>(REGISTER_ROOM_BIND(room_t::c_create_entity));
}

void room_t::broadcast(google::protobuf::Message& msg) {
	for (int32 k = 0; k < 2; k++) {
		int64 uid = camps_[k];
		if (uid > 0) {
			get_service()->send(uid, msg);
		}
	}
}

void room_t::c_get_room_info(void *player, const battle_msg::c_get_room_info& msg) {
	player_t *p = (player_t *)player;
	battle_msg::s_get_room_info res;
	res.set_res("ok");
	battle_msg::room_info *info = res.mutable_info();
	pack_room_info(info);
	battle_msg::entity_info *entity_info;
	for (object_mgr_base_t::object_map_t::iterator it = entity_mgr_->objects_.begin(); it != entity_mgr_->objects_.end(); it++) {
		entity_t *entity = (entity_t *)(it->second);
		entity_info = res.add_entitys();
		pack_entity_info(entity, entity_info);
	}
	battle_msg::bullet_info *bullet_info;
	for (object_mgr_base_t::object_map_t::iterator it = bullet_mgr_->objects_.begin(); it != bullet_mgr_->objects_.end(); it++) {
		bullet_t *bullet = (bullet_t *)(it->second);
		bullet_info = res.add_bullets();
		pack_bullet_info(bullet, bullet_info);
	}
	get_service()->send(p->get_uid(), res);
	LOG("%s", res.DebugString().c_str());
}

void room_t::c_create_entity(void *player, const battle_msg::c_create_entity& msg) {
	player_t *p = (player_t *)player;
	if (room_state_ != ROOM_STATE_ING) {
		return;
	}
	LOG("%d  %d  %d", msg.type_id(), msg.row(), msg.col());
	entity_mgr_->create_entity(p->get_camp(), msg.type_id(), msg.row(), msg.col());
}

void room_t::start_wait() {
	ERROR("room wait");
	room_state_ = ROOM_STATE_WAIT;
	this->on_room_state_change(room_state_);
	register_timer_delay(TIMER_CALLBACK(room_t::start_wait_timeout), ROOM_WAIT_TIME);
}

void room_t::start_ing() {
	ERROR("room ing");
	room_state_ = ROOM_STATE_ING;
	this->on_room_state_change(room_state_);
	register_timer_delay(TIMER_CALLBACK(room_t::start_ing_timeout), ROOM_ING_TIME);
}

void room_t::start_end() {
	ERROR("room end");
	room_state_ = ROOM_STATE_END;
	this->on_room_state_change(room_state_);
	register_timer_delay(TIMER_CALLBACK(room_t::start_end_timeout), ROOM_END_TIME);
}

void room_t::start_wait_timeout(void *data) {
	start_ing();
}

void room_t::start_ing_timeout(void *data) {
	start_end();
}

void room_t::start_end_timeout(void *data) {
	set_del(true);
	// kick player
	for (int k = 0; k < 2; k++) {
		if (camps_[k] != 0) {
			get_player_mgr()->kick(camps_[k]);
			camps_[k] = 0;
		}
	}
}

void room_t::pack_entity_info(entity_t *entity, battle_msg::entity_info *info) {
	info->set_id(entity->get_id());
	info->set_type_id(entity->get_type_id());
	info->set_camp(entity->get_camp());
	info->set_blood(entity->get_blood());
	info->set_cd(entity->get_cd());
	info->set_row(entity->get_row());
	info->set_col(entity->get_col());
	battle_msg::vector *pos = info->mutable_pos();
	pos->set_x(entity->get_pos().x);
	pos->set_y(entity->get_pos().y);
	pos->set_z(entity->get_pos().z);
}

void room_t::pack_bullet_info(bullet_t *bullet, battle_msg::bullet_info *info) {
	info->set_id(bullet->get_id());
	info->set_type_id(bullet->get_type_id());
	info->set_camp(bullet->get_camp());
	info->set_damage(bullet->get_damage());
	info->set_begin_time(bullet->get_begin_time());
	info->set_path(bullet->get_path());
	battle_msg::vector *begin_pos = info->mutable_begin_pos();
	begin_pos->set_x(bullet->get_begin_pos().x);
	begin_pos->set_y(bullet->get_begin_pos().y);
	begin_pos->set_z(bullet->get_begin_pos().z);
	battle_msg::vector *pos = info->mutable_pos();
	pos->set_x(bullet->get_pos().x);
	pos->set_y(bullet->get_pos().y);
	pos->set_z(bullet->get_pos().z);
	battle_msg::vector *begin_speed = info->mutable_begin_speed();
	begin_speed->set_x(bullet->get_begin_v_speed().x);
	begin_speed->set_y(bullet->get_begin_v_speed().y);
	begin_speed->set_z(bullet->get_begin_v_speed().z);
	battle_msg::vector *speed = info->mutable_speed();
	speed->set_x(bullet->get_v_speed().x);
	speed->set_y(bullet->get_v_speed().y);
	speed->set_z(bullet->get_v_speed().z);
}

void room_t::pack_room_info(battle_msg::room_info *info) {
	info->set_state(room_state_);
	info->set_begin_time(begin_time_);
	info->set_now_time(getfs());
}

void room_t::on_room_state_change(int32 state) {
	battle_msg::s_room_state msg;
	battle_msg::room_info *info = msg.mutable_info();
	pack_room_info(info);
	broadcast(msg);
}

void room_t::on_create_entity(entity_t *entity) {
	battle_msg::s_create_entity msg;
	battle_msg::entity_info *info = msg.mutable_einfo();
	pack_entity_info(entity, info);
	msg.set_use_gold(0);
	msg.set_cur_gold(0);
	broadcast(msg);
}

void room_t::on_del_entity(entity_t *entity) {
	battle_msg::s_destroy_entity msg;
	msg.set_eid(entity->get_id());
	broadcast(msg);
}

void room_t::on_create_bullet(entity_t *entity, bullet_t *bullet) {

}

void room_t::on_del_bullet(bullet_t *bullet) {

}

void room_t::on_entity_damage(entity_t *entity, int32 damage) {

}

void room_t::on_fire(entity_t *entity, bullet_t *bullet) {
	battle_msg::s_fire msg;
	msg.set_eid(entity->get_id());
	battle_msg::bullet_info *info = msg.mutable_binfo();
	pack_bullet_info(bullet, info);
	broadcast(msg);
}

void room_t::on_collision(entity_t *entity, bullet_t *bullet) {
	battle_msg::s_collision msg;
	battle_msg::entity_info *einfo = msg.mutable_einfo();
	pack_entity_info(entity, einfo);
	battle_msg::bullet_info *binfo = msg.mutable_binfo();
	pack_bullet_info(bullet, binfo);
	msg.set_bullet_destroy(true);
	broadcast(msg);
}