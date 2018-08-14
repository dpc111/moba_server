#include "id_generate.h"

id_generate_t::id_generate_t() {
	cur_ = 0;
}

id_generate_t::~id_generate_t() {
	cur_ = 0;
}

int id_generate_t::gen() {
	cur_++;
	return cur_;
}
