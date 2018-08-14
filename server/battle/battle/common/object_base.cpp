#include "object_base.h"

object_base_t::object_base_t(int32 id, int32 type_id) {
	id_ = id;
	type_id_ = type_id;
}

object_base_t::~object_base_t() {
	id_ = 0;
	type_id_ = 0;
}