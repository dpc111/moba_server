#ifndef ID_GENERATE_H
#define ID_GENERATE_H

#include "stdafx.h"

class id_generate_t {
public:
	id_generate_t();

	~id_generate_t();

	int gen();

private:
	int32 cur_;
};

#endif