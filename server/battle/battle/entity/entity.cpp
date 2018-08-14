#include "entity.h"
#include "room.h"
#include "bullet.h"
#include "bullet_mgr.h"
#include "bullet.h"
#include "grid.h"
#include "state_cache.h"

entity_t::entity_t(room_t *room, int32 entity_id, int32 type_id) :
	object_base_t(entity_id, type_id),
	room_(room) {
	camp_ = 1;
	level_ = 1;
	cd_ = 1;
	blood_ = 0;
	bullet_id_ = 0;
	damage_ = 0;
	frozen_ = 0;
	row_ = 0;
	col_ = 0;
	state_ = ENTITY_STATE_NONE;
	last_state_time_ = 0;
}

entity_t::~entity_t() {

}

void entity_t::update_state(int32 state) {
	if (state_ == state) {
		return;
	}
	set_state(state);
	set_last_state_time(getfs());
	room_->get_state_cache()->add_cache(state, get_id());
}

void entity_t::update(double tm) {
	switch (state_) {
		case ENTITY_STATE_NONE:
			update_state(ENTITY_STATE_BORN);
			break;

		case ENTITY_STATE_BORN:
			if (tm - last_state_time_ > born_time_) {
				update_state(ENTITY_STATE_IDLE);
			}
			break;

		case ENTITY_STATE_IDLE:
			if (tm - last_state_time_ > cd_) {
				before_fire();
			}
			break;

		case ENTITY_STATE_FIRE:
			if (tm - last_state_time_ > fire_before_time_) {
				fire();
				update_state(ENTITY_STATE_IDLE);
			}
			break;

		case ENTITY_STATE_DEATH:
			if (tm - last_state_time_ > death_time_) {
				update_state(ENTITY_STATE_DEL);
			}
			break;

		case ENTITY_STATE_DEL:
			break;

		default:
			break;
	}
}

void entity_t::update_gun_state() {
	entity_t *entity = room_->get_grid()->pick_enemy(camp_, col_);
	if (! entity) {
		target_ = 0;
		return;
	}
	target_ = entity->get_id();
	if (bullet_path_ == BULLET_PATCH_PARABOLA) {
		vector3_t& pos = entity->get_pos();
		float vx = FABS(pos.x - pos_.x) / bullet_life_time_;
		if (IS_LEFT_CAMP(camp_)) {
			vx = vx;
		} else {
			vx = -vx;
		}
		bullet_begin_speed_.x = vx;
	}
}

// 前摇
void entity_t::before_fire() {
	update_gun_state();
	if (target_ == 0) {
		return;
	}
	update_state(ENTITY_STATE_FIRE);
}

// 开火
void entity_t::fire() {
	update_gun_state();
	if (target_ == 0) {
		return;
	}
	bullet_t *bullet = room_->get_bullet_mgr()->create_bullet(this, bullet_id_);
	room_->on_fire(this, bullet);
}

void entity_t::death() {
	update_state(ENTITY_STATE_DEATH);
}

void entity_t::on_collision(bullet_t *bullet) {
	int32 damage = bullet->get_damage();
	blood_ = blood_ - damage;
	room_->on_entity_damage(this, damage);
	if (blood_ <= 0) {
		blood_ = 0;
		death();
	}
}