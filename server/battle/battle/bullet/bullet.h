#ifndef BULLET_H
#define BULLET_H

#include "stdafx.h"
#include "object_base.h"
#include "grid.h"
#include "gmath.h"
#include "entity.h"

class room_t;

class bullet_t : public object_base_t {
public:
	bullet_t(room_t *room, int32 bullet_id, int32 type_id);

	virtual ~bullet_t();

	virtual void init(entity_t * entity);

	virtual void update(double stm);

	void update_state(int32 state);

	void on_collision(entity_t *entity);

	void on_bullet_out();

protected:
	PROPERTY_PRO(room_t*, 				room)
	PROPERTY_PRO(int32, 				speed)
	PROPERTY_PRO(int32, 				path)
	PROPERTY_PRO(int32, 				damage)
	PROPERTY_PRO(float, 				damage_r)
	PROPERTY_PRO(int32, 				camp)
	PROPERTY_PRO(int32, 				line)
	PROPERTY_PRO(double, 				begin_time)
	PROPERTY_PRO_QUOTE(vector3_t, 		begin_v_speed)
	PROPERTY_PRO_QUOTE(vector3_t, 		v_speed)
	PROPERTY_PRO_QUOTE(vector3_t, 		begin_pos)
	PROPERTY_PRO_QUOTE(vector3_t, 		pos)
	PROPERTY_PRO(int32,					state)
	PROPERTY_PRO(double,				last_state_time)
};

#endif