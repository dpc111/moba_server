#ifndef GRID_H
#define GRID_H

#define ROW_NUM 10
#define COL_NUM 5
#define ROW_LENGTH 10
#define COL_LENGTH 10
#define BULLET_OUT_MIN_X 0
#define BULLET_OUT_MAX_X 100
#define BULLET_OUT_MIN_Z 0
#define BULLET_OUT_MAX_Z 50
#define BULLET_OUT_MIN_Y 0
#define BULLET_OUT_MAX_Y 200
#define LEFT_CAMP 1
#define RIGHT_CAMP 2
#define IS_LEFT_CAMP(camp) (camp == LEFT_CAMP)
#define IS_RIGHT_CAMP(camp) (camp == RIGHT_CAMP)

#include "stdafx.h"
#include "gmath.h"

class room_t;
class entity_t;
class bullet_t;

class grid_t {
public:
	grid_t(room_t *room);

	~grid_t();

	void set_entity(int32 row, int32 col, entity_t *entity);

	void del_entity(int32 row, int32 col);

	entity_t* get_entity(int32 row, int32 col);

	bool exist(int32 row, int32 col);

	vector3_t& get_pos(int32 row, int32 col);

	entity_t* pick_enemy(int32 camp, int32 line);

	void process_collision(bullet_t *bullet);

public:
	entity_t* grids_[ROW_NUM][COL_NUM];

	vector3_t pos_[ROW_NUM][COL_NUM];

	room_t* room_;

	collision_cube_simple_t collision_box_;
};

#endif