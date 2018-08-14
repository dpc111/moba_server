#ifndef BULLET_PARABOLA_H
#define BULLET_PARABOLA_H

#include "bullet.h"
#include "room.h"

class bullet_parabola_t : public bullet_t {
public:
	bullet_parabola_t(room_t *room, int32 bullet_id, int32 type_id);

	virtual ~bullet_parabola_t();

	virtual void init(entity_t * entity);

	virtual void update(double stm);

private:
	
};

#endif