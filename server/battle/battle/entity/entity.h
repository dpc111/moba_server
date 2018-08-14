#ifndef ENTITY_H
#define ENTITY_H

#include "object_base.h"

class room_t;
class bullet_t;

class entity_t : public object_base_t {
public:
	entity_t(room_t *room, int32 entity_id, int32 type_id);

	virtual ~entity_t();

	void update_state(int32 state);

	void update(double tm);

	void update_gun_state();

	void before_fire();

	void fire();

	void death();

	void on_collision(bullet_t *bullet);

private:
	PROPERTY(room_t*, 							room)
	PROPERTY_QUOTE(vector3_t, 					pos)
	PROPERTY_QUOTE(vector3_t, 					gun_pos)
	PROPERTY_QUOTE(collision_cube_simple_t,		collision_box)
	PROPERTY(int32,								camp)
	PROPERTY(int32,								level)
	PROPERTY(float,								cd)
	PROPERTY(int32,								blood)
	PROPERTY(int32,								bullet_path)
	PROPERTY(int32,								bullet_id)
	PROPERTY(int32,								damage)
	PROPERTY(float,								frozen)
	PROPERTY(float,								born_time)
	PROPERTY(float,								fire_before_time)
	PROPERTY(float,								death_time)
	PROPERTY(int32,								row)
	PROPERTY(int32,								col)

	PROPERTY(int32,								target)
	PROPERTY(float,								bullet_life_time)
	PROPERTY_QUOTE(vector3_t,					bullet_begin_speed)
	PROPERTY(int32,								state)
	PROPERTY(double,							last_state_time)
};

#endif