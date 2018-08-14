#ifndef ENTITY_MGR_H
#define ENTITY_MGR_H

#include "stdafx.h"
#include "object_mgr_base.h"

class entity_t;
class room_t;

class entity_mgr_t : public object_mgr_base_t {
friend class room_t;
	
public:
	entity_mgr_t(room_t *room);

	virtual ~entity_mgr_t();

	entity_t *create_entity(int32 camp, int32 type_id, int32 row, int32 col);

	void delete_entity(int32 entity_id);

	void update(double stm);

private:
	room_t *room_;
	
};

#endif