#include "entity_mgr.h"
#include "entity.h"
#include "global.h"
#include "room.h"
#include "grid.h"

entity_mgr_t::entity_mgr_t(room_t *room) :
	object_mgr_base_t(),
	room_(room) {

}

entity_mgr_t::~entity_mgr_t() {

}

entity_t *entity_mgr_t::create_entity(int32 camp, int32 type_id, int32 row, int32 col) {
	int32 entity_id = gen_id();
	if (get_object(entity_id)) {
		return NULL;
	}
	if (!get_json_mgr()->exist("entity", type_id - 1)) {
		return NULL;
	}
	entity_t *entity = new entity_t(room_, entity_id, type_id);
	entity->set_pos(room_->get_grid()->get_pos(row, col));
	room_->get_grid()->set_entity(row, col, entity);
	float gun_posx = get_json_mgr()->get_float("entity", type_id - 1, "gun_pos", 0);
	float gun_posy = get_json_mgr()->get_float("entity", type_id - 1, "gun_pos", 1);
	float gun_posz = get_json_mgr()->get_float("entity", type_id - 1, "gun_pos", 2);
	if (IS_LEFT_CAMP(camp)) {
		gun_posx = gun_posx;
		gun_posy = gun_posy;
		gun_posz = gun_posz;
	} else {
		gun_posx = -gun_posx;
		gun_posy = gun_posy;
		gun_posz = gun_posz;
	}
	entity->set_gun_pos(vector3_t(gun_posx, gun_posy, gun_posz));
	entity->set_camp(camp);
	entity->set_level(1);
	entity->set_cd(get_json_mgr()->get_float("entity", type_id - 1, "cd"));
	entity->set_blood(get_json_mgr()->get_int("entity", type_id - 1, "blood"));
	entity->set_bullet_id(get_json_mgr()->get_int("entity", type_id - 1, "bullet"));
	entity->set_damage(get_json_mgr()->get_float("entity", type_id - 1, "damage"));
	entity->set_born_time(get_json_mgr()->get_float("entity", type_id - 1, "born_time"));
	entity->set_fire_before_time(get_json_mgr()->get_float("entity", type_id - 1, "fire_befor_time"));
	entity->set_death_time(get_json_mgr()->get_float("entity", type_id - 1, "death_time"));
	entity->set_row(row);
	entity->set_col(col);
	entity->set_bullet_path(get_json_mgr()->get_float("bullet", entity->get_bullet_id() - 1, "path"));
	float high = get_json_mgr()->get_float("bullet", entity->get_bullet_id() - 1, "high");
	entity->set_bullet_life_time(sqrt(2 * high / GRAVITY) * 2.0);
	entity->set_bullet_begin_speed(vector3_t(0, (entity->get_bullet_life_time() / 2.0) * GRAVITY, 0));
	float boxx = get_json_mgr()->get_float("entity", type_id - 1, "box", 0);
	float boxy = get_json_mgr()->get_float("entity", type_id - 1, "box", 1);
	float boxz = get_json_mgr()->get_float("entity", type_id - 1, "box", 2);
	entity->set_collision_box(collision_cube_simple_t(entity->get_pos(), boxx, boxy, boxz));
	add_object(entity_id, entity);
	room_->on_create_entity(entity);
	return entity;
}

void entity_mgr_t::delete_entity(int32 entity_id) {
	entity_t *entity = (entity_t *)get_object(entity_id);
	if (!entity) {
		return;
	}
	room_->get_grid()->del_entity(entity->get_row(), entity->get_col());
	room_->on_del_entity(entity);
	delete entity;
	del_object(entity_id);
}

void entity_mgr_t::update(double stm) {
	for (object_map_t::iterator it = objects_.begin(); it != objects_.end(); ) {
		entity_t *entity = (entity_t *)(it->second);
		if (entity->get_state() == ENTITY_STATE_DEL) {
			++it;
			delete_entity(entity->get_id());
			continue;
		}
		entity->update(stm);
		++it;
	}
}