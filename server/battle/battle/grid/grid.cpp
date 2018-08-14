#include "grid.h"
#include "room.h"
#include "entity.h"
#include "bullet.h"

grid_t::grid_t(room_t *room) :
	room_(room),
	collision_box_(BULLET_OUT_MIN_X, BULLET_OUT_MAX_X, BULLET_OUT_MIN_Y, BULLET_OUT_MAX_Y, BULLET_OUT_MIN_Z, BULLET_OUT_MAX_Z) {
	for (int i = 0; i < ROW_NUM; i++) {
		for (int j = 0; j < COL_NUM; j++) {
			grids_[i][j] = NULL;
		}
	}
	for (int i = 0; i < ROW_NUM; i++) {
		for (int j = 0; j < COL_NUM; j++) {
			int x = i * ROW_LENGTH + (ROW_LENGTH / 2);
			int z = j * COL_LENGTH + (COL_LENGTH / 2);
			pos_[i][j] = vector3_t(x, 3, z);	
		}
	}
}

grid_t::~grid_t() {
	for (int i = 0; i < ROW_NUM; i++) {
		for (int j = 0; j < COL_NUM; j++) {
			grids_[i][j] = NULL;
		}
	}
}

void grid_t::set_entity(int32 row, int32 col, entity_t *entity) {
	if (row < 1 || row > ROW_NUM || col < 1 || col > COL_NUM) {
		return;
	}
	if (exist(row, col)) {
		return;
	}
	grids_[row - 1][col - 1] = entity;
}

void grid_t::del_entity(int32 row, int32 col) {
	if (row < 1 || row > ROW_NUM || col < 1 || col > COL_NUM) {
		return;
	}
	if (!exist(row, col)) {
		return;
	}
	grids_[row - 1][col - 1] = NULL;
}

entity_t* grid_t::get_entity(int32 row, int32 col) {
	if (row < 1 || row > ROW_NUM || col < 1 || col > COL_NUM) {
		return NULL;
	}
	return grids_[row - 1][col - 1];
}

bool grid_t::exist(int32 row, int32 col) {
	if (row < 1 || row > ROW_NUM || col < 1 || col > COL_NUM) {
		return false;
	}
	if (grids_[row - 1][col - 1]) {
		return true;
	}
	return false;
}

vector3_t& grid_t::get_pos(int32 row, int32 col) {
	assert(pos_[row - 1][col - 1]);
	return pos_[row - 1][col - 1];
}

entity_t* grid_t::pick_enemy(int32 camp, int32 line) {
	--line;
	if (IS_LEFT_CAMP(camp)) {
		for (int i = 0; i < ROW_NUM; i++) {
			entity_t *entity = grids_[i][line];
			if (entity && entity->get_camp() != camp) {
				return entity;
			}
		}
	} else {
		for (int i = ROW_NUM - 1; i >= 0; i--) {
			entity_t * entity = grids_[i][line];
			if (entity && entity->get_camp() != camp) {
				return entity;
			}
		}
	}
	return NULL;
}

void grid_t::process_collision(bullet_t *bullet) {
	int32 camp = bullet->get_camp();
	int32 line = bullet->get_line() - 1;
	vector3_t& pos = bullet->get_pos();
	if (!collision_check(pos, collision_box_)) {
		bullet->on_bullet_out();
		return;
	}
	for (int32 k = 0; k < ROW_NUM; k++) {
		entity_t* entity = grids_[k][line];
		if (entity &&
			entity->get_camp() != camp &&
			collision_check(pos, entity->get_collision_box())) {
			entity->on_collision(bullet);
			bullet->on_collision(entity);
			room_->on_collision(entity, bullet);
			return;
		}
	}
}