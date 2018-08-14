#include "bullet_mgr.h"
#include "bullet.h"
#include "global.h"
#include "room.h"
#include "entity.h"
#include "grid.h"
#include "bullet_horizontal.h"
#include "bullet_parabola.h"

bullet_mgr_t::bullet_mgr_t(room_t *room) :
	object_mgr_base_t(),
	room_(room) {

}

bullet_mgr_t::~bullet_mgr_t() {

}

bullet_t *bullet_mgr_t::create_bullet(entity_t *entity, int32 type_id) {
	int32 bullet_id = gen_id();
	if (get_object(bullet_id)) {
		ERROR("");
		return NULL;
	}
	if (!get_json_mgr()->exist("bullet", type_id - 1)) {
		ERROR("");
		return NULL;
	}

	bullet_t *bullet = NULL;
	int32 path = get_json_mgr()->get_int("bullet", type_id - 1, "path");
	if (path == BULLET_PATCH_HORIZONTAL) {
		bullet = new bullet_horizontal_t(room_, bullet_id, type_id);
	} else if (path == BULLET_PATCH_PARABOLA) {
		bullet = new bullet_parabola_t(room_, bullet_id, type_id);
	} else {
		return NULL;
	}
	bullet->init(entity);

	add_object(bullet_id, bullet);
	room_->on_create_bullet(entity, bullet);
	return bullet;
}

void bullet_mgr_t::delete_bullet(int32 bullet_id) {
	bullet_t *bullet = (bullet_t *)get_object(bullet_id);
	if (!bullet) {
		return;
	}
	room_->on_del_bullet(bullet);
	delete bullet;
	del_object(bullet_id);
}

void bullet_mgr_t::update(double stm) {
	for (object_map_t::iterator it = objects_.begin(); it != objects_.end(); ) {
		bullet_t *bullet = (bullet_t *)(it->second);
		if (bullet->get_state() == BULLET_STATE_DEL) {
			++it;
			delete_bullet(bullet->get_id());
			continue;
		} 
		bullet->update(stm);
		++it;
	}
}