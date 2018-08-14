#ifndef COLLISION_H
#define COLLISION_H

#include "gmath.h"

struct collision_sphere_t {
	vector3_t center;
	float radius;

	collision_sphere_t() {}

	collision_sphere_t(const vector3_t& center_, float radius_) : 
		center(center_), 
		radius(radius_) {
	}

	collision_sphere_t(const collision_sphere_t& v) {
		center = v.center;
		radius = v.radius;
	}

	collision_sphere_t& operator= (const collision_sphere_t& v) {
		center = v.center;
		radius = v.radius;
		return *this;
	}
};

struct collision_cube_simple_t {
	float x1;
	float x2;
	float y1;
	float y2;
	float z1;
	float z2;

	collision_cube_simple_t() {}

	collision_cube_simple_t(const vector3_t& pos, float x, float y, float z) {
		x1 = pos.x - x;		
		x2 = pos.x + x;
		y1 = pos.y - y;		
		y2 = pos.y + y;	
		z1 = pos.z - z;		
		z2 = pos.z + z;			
	}

	collision_cube_simple_t(float x1_, float x2_, float y1_, float y2_, float z1_, float z2_) : 
		x1(x1_), 
		x2(x2_), 
		y1(y1_), 
		y2(y2_), 
		z1(z1_), 
		z2(z2_) {
	}

	collision_cube_simple_t(const collision_cube_simple_t& v) {
		x1 = v.x1;
		x2 = v.x2;
		y1 = v.y1;
		y2 = v.y2;
		z1 = v.z1;
		z2 = v.z2;
	}

	collision_cube_simple_t& operator= (const collision_cube_simple_t& v) {
        x1 = v.x1;
		x2 = v.x2;
		y1 = v.y1;
		y2 = v.y2;
		z1 = v.z1;
		z2 = v.z2;
        return *this;
    }
};

struct collision_cylinder_simple_t {
	vector3_t center;
	float radius;
	float high;

	collision_cylinder_simple_t() {}

	collision_cylinder_simple_t(const vector3_t& center_, float radius_, float high_) : 
		center(center_), 
		radius(radius_), 
		high(high_) {
	}

	collision_cylinder_simple_t(const collision_cylinder_simple_t& v) {
		center = v.center;
		radius = v.radius;
		high = v.high;
	}

	collision_cylinder_simple_t& operator= (const collision_cylinder_simple_t& v) {
		center = v.center;
		radius = v.radius;
		high = v.high;
		return *this;
	}
};

bool collision_check(const vector3_t& pos, const collision_sphere_t& sphere);

bool collision_check(const vector3_t& pos, const collision_cube_simple_t& cube);

bool collision_check(const vector3_t& pos, const collision_cylinder_simple_t& cylinder);

#endif