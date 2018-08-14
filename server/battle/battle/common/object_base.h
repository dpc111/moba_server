#ifndef OBJECT_BASE_H
#define OBJECT_BASE_H

#include "stdafx.h"

#define PROPERTY(type, property)\
private:\
	type property##_;\
public:\
	inline type get_##property() { return property##_; }\
public:\
	inline void set_##property(type value) { property##_ = value; }\

#define PROPERTY_QUOTE(type, property)\
private:\
	type property##_;\
public:\
	inline type & get_##property() { return property##_; }\
public:\
	inline void set_##property(const type & value) { property##_ = value; }\

#define PROPERTY_PRO(type, property)\
protected:\
	type property##_;\
public:\
	inline type get_##property() { return property##_; }\
public:\
	inline void set_##property(type value) { property##_ = value; }\

#define PROPERTY_PRO_QUOTE(type, property)\
protected:\
	type property##_;\
public:\
	inline type & get_##property() { return property##_; }\
public:\
	inline void set_##property(const type & value) { property##_ = value; }\

class object_base_t {
public:
	object_base_t(int32 id, int32 type_id);

	virtual	~object_base_t();

	PROPERTY_PRO(int32, type_id)

	PROPERTY_PRO(int32, id)
};

#endif