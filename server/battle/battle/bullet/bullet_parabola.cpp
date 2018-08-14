#include "bullet_parabola.h"

bullet_parabola_t::bullet_parabola_t(room_t *room, int32 bullet_id, int32 type_id) : 
	bullet_t(room, bullet_id, type_id) {

}

bullet_parabola_t::~bullet_parabola_t() {

}

void bullet_parabola_t::init(entity_t * entity) {
	bullet_t::init(entity);
	set_v_speed(entity->get_bullet_begin_speed());
	set_begin_v_speed(entity->get_bullet_begin_speed());
}

void bullet_parabola_t::update(double stm) {
	if (state_ == BULLET_STATE_DEL) {
		return;
	}
	if (state_ == BULLET_STATE_FLY) {
		double interval = stm - last_state_time_;
		v_speed_.y = begin_v_speed_.y - GRAVITY * interval;
		pos_.x = begin_pos_.x + begin_v_speed_.x * interval;
		pos_.y = begin_pos_.y + begin_v_speed_.y * interval - 0.5 * GRAVITY * interval * interval;
		pos_.z = pos_.z;	
	}
	bullet_t::update(stm);
}