#include <math.h>
#include "vector2.h"
#include "vector3.h"

#define FABS(x) fabs(x)
#define SQRT(x) sqrt(x)
#define GRAVITY 0.98
#define SQUARE(x) ((x)*(x))
#define SQUARE_DISTANCE(pos1, pos2) (SQUARE(pos1.x - pos2.x) + SQUARE(pos1.x - pos2.x) + SQUARE(pos1.x - pos2.x))

float square_distance(const vector3_t& pos1, const vector3_t& pos2);