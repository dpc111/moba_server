#ifndef TIEMSTAMP_H
#define TIEMSTAMP_H

#include "type.h"

// int64 timestamp_gettimeofday() {
// 	timeval tv;
// 	gettimeofday(&tv, NULL);
// 	return 1000000 * int64(tv.tv_sec) + int64(tv.tv_usec)
// }

int64 gettime();

int64 getus();

int64 getms();

double getfs();

void sleepms(int64 ms);

#endif
