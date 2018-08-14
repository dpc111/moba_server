#include "gmath.h"

float square_distance(const vector3_t& pos1, const vector3_t& pos2) {
	return SQUARE(pos1.x - pos2.x) + SQUARE(pos1.x - pos2.x) + SQUARE(pos1.x - pos2.x);
}