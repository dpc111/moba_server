#include "bullet_horizontal.h"

bullet_horizontal_t::bullet_horizontal_t(room_t	*room, int32 bullet_id, int32 type_id) 
	: bullet_t(room, bullet_id, type_id) {

}

bullet_horizontal_t::~bullet_horizontal_t() {

}

void bullet_horizontal_t::init(entity_t * entity) {
	bullet_t::init(entity);
	if (IS_LEFT_CAMP(entity->get_camp())) {
		set_v_speed(vector3_t(get_speed(), 0, 0));
		set_begin_v_speed(vector3_t(get_speed(), 0, 0));
	} else {
		set_v_speed(-vector3_t(get_speed(), 0, 0));
		set_begin_v_speed(-vector3_t(get_speed(), 0, 0));
	}
}

void bullet_horizontal_t::update(double stm) {
	if (state_ == BULLET_STATE_DEL) {
		return;
	}
	if (state_ == BULLET_STATE_FLY) {
		v_speed_ = begin_v_speed_;
		double interval = stm - last_state_time_;
		pos_ = begin_pos_ + v_speed_ * interval;
	}
	bullet_t::update(stm);
}