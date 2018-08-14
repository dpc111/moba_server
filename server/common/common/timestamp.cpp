#include "timestamp.h"
#include "stdio.h"
#include <sys/time.h>
#include <time.h>
#include <asm/unistd.h>
#include "type.h"

int64 gettime() {
	int64 tm;
	timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	tm = int64(ts.tv_sec) * 1000000 + ts.tv_nsec / 1000;
	return tm;
}

int64 getus() {
	return gettime();
}

int64 getms() {
	return getus() / 1000;
}

double getfs() {
	return (double)getus() / 1000000;
}

int64 getis() {
	return getus() / 1000000;
}

void sleepms(int64 ms) {
	struct timeval tv;
	tv.tv_sec = ms / 1000;
	tv.tv_usec = ms % 1000 * 1000;
	if (select(0, NULL, NULL, NULL, &tv) == -1) {
		printf("sleep fail\n");
	}
} 
