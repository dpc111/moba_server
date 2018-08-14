#include "dispatcher.h"
#include "log.h"

dispatcher_t::dispatcher_t() {
	this->register_callback();
}

dispatcher_t::~dispatcher_t() {
	for (msg_map_t::iterator it = msg_map_.begin(); it != msg_map_.end(); it++) {
		delete it->second;
	}
	msg_map_.clear();
}

void dispatcher_t::on_message(void *p, const google::protobuf::Descriptor* des, const google::protobuf::Message *msg) {
	msg_map_t::iterator it = msg_map_.find(des);
	if (it != msg_map_.end()) {
		cb_t *cb = it->second;
		cb->on_message(p, msg);
	}
}