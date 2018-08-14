#include "object_mgr_base.h"
#include "object_base.h"

object_mgr_base_t::object_mgr_base_t() {
	objects_.clear();
	object_num_ = 0;
	id_generate_ = id_generate_t();
}

object_mgr_base_t::~object_mgr_base_t() {
	for (object_map_t::iterator it = objects_.begin(); it != objects_.end(); it++) {
		object_base_t *object = it->second;
		delete object;
	}
	objects_.clear();
	object_num_ = 0;
}

object_base_t *object_mgr_base_t::get_object(int32 object_id) {
	object_map_t::iterator it = objects_.find(object_id);
	if (it == objects_.end()) {
		return NULL;
	}
	return it->second;	
}

void object_mgr_base_t::add_object(int32 object_id, object_base_t *object) {
	if (get_object(object_id)) {
		return;
	}
	objects_[object_id] = object;
	object_num_++;
}

void object_mgr_base_t::del_object(int32 object_id) {
	object_map_t::iterator it = objects_.find(object_id);
	if (it == objects_.end()) {
		return;
	}
	objects_.erase(it);
	object_num_--;
}