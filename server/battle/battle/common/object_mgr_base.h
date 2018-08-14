#ifndef OBJECT__MGR_BASE_H
#define OBJECT__MGR_BASE_H

#include <map>
#include "stdafx.h"
#include "id_generate.h"

class object_base_t;

class object_mgr_base_t {
public:
	typedef std::map<int32, object_base_t *> object_map_t;

public:
	object_mgr_base_t();

	virtual ~object_mgr_base_t();

	object_base_t *get_object(int32 object_id);

	void add_object(int32 object_id, object_base_t *obj);

	void del_object(int32 object_id);

	int32 get_object_num() { return object_num_; }

	object_map_t& get_objects() { return objects_; }

	int32 gen_id() { return id_generate_.gen(); }

public:
	object_map_t objects_;

	int32 object_num_;

	id_generate_t id_generate_;
};

#endif