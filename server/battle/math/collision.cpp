#include "collision.h"

bool collision_check(const vector3_t& pos, const collision_sphere_t& sphere) {
	if (SQUARE_DISTANCE(pos, sphere.center) < SQUARE(sphere.radius)) {
		return true;
	}
	return false;
}

bool collision_check(const vector3_t& pos, const collision_cube_simple_t& cube) {
	if (pos.x > cube.x1 &&
		pos.x < cube.x2 &&
		pos.y > cube.y1 &&
		pos.y < cube.y2 &&
		pos.z > cube.z1 &&
		pos.z < cube.z2) {
		return true;
	}
	return false;
}

bool collision_check(const vector3_t& pos, const collision_cylinder_simple_t& cylinder) {
	if (pos.y < cylinder.center.y ||
		pos.y > cylinder.center.y + cylinder.high) {
		return false;
	}
	if (SQUARE(pos.x - cylinder.center.x) + SQUARE(pos.z - cylinder.center.z) > SQUARE(cylinder.radius)) {
		return false;
	}
	return true;
}