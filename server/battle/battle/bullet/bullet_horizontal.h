#ifndef BULLET_HORIZONTAL_H
#define BULLET_HORIZONTAL_H

#include "bullet.h"
#include "room.h"

class bullet_horizontal_t : public bullet_t {
public:
	bullet_horizontal_t(room_t *room, int32 bullet_id, int32 type_id);

	virtual ~bullet_horizontal_t();

	virtual void init(entity_t * entity);

	virtual void update(double stm);

private:

};

#endif