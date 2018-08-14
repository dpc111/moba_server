#ifndef LOG_H
#define LOG_H

#include "stdio.h"

#define ERROR(fmt, ...) printf("\033[1;35m[%s][%d]", __FILE__, __LINE__); printf(fmt, ##__VA_ARGS__); printf("\n\033[0m")
#define WARN(fmt, ...) printf("\033[1;33m[%s][%d]", __FILE__, __LINE__); printf(fmt, ##__VA_ARGS__); printf("\n\033[0m")
#define LOG(fmt, ...) printf("\033[37m[%s][%d]", __FILE__, __LINE__); printf(fmt, ##__VA_ARGS__); printf("\n\033[0m")

#endif
