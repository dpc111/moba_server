#include "cfg_json_mgr.h"
#include "cfg_json.h"
#include "log.h"

cfg_json_mgr_t::cfg_json_mgr_t() {
	reader_ = new Json::Reader();	
}

cfg_json_mgr_t::~cfg_json_mgr_t() {
	delete reader_;
}

void cfg_json_mgr_t::load(std::string file_name, std::string json_name) {
	cfg_json_t *json = new cfg_json_t(this);
	if (json->init(file_name.c_str())) {
		WARN("load json file: %s", file_name.c_str());
		jsons_[json_name] = json;
	} else {
		ERROR("load json file: %s failed", file_name.c_str());
		delete json;
	}
}

cfg_json_t* cfg_json_mgr_t::get_json(std::string json_name) {
	cfg_json_map_t::iterator it = jsons_.find(json_name);
	if (it == jsons_.end()) {
		ERROR("%s", json_name.c_str());
		return NULL;
	}
	return it->second;
}

bool cfg_json_mgr_t::exist(std::string json_name, int32 key1) {
	cfg_json_t *json = this->get_json(json_name);
	if (!json) {
		return false;
	}
	return json->exist(key1);
}

int cfg_json_mgr_t::get_int(std::string json_name, const char *key1) {
	cfg_json_t *json = this->get_json(json_name);
	if (!json) {
		return JSON_ERROR_INT;
	}
	return json->get_int(key1);
}

int cfg_json_mgr_t::get_int(std::string json_name, int key1) {
	cfg_json_t *json = this->get_json(json_name);
	if (!json) {
		return JSON_ERROR_INT;
	}
	return json->get_int(key1);
}

int cfg_json_mgr_t::get_int(std::string json_name, const char *key1, const char *key2) {
	cfg_json_t *json = this->get_json(json_name);
	if (!json) {
		return JSON_ERROR_INT;
	}
	return json->get_int(key1, key2);
}

int cfg_json_mgr_t::get_int(std::string json_name, const char *key1, int key2) {
	cfg_json_t *json = this->get_json(json_name);
	if (!json) {
		return JSON_ERROR_INT;
	}
	return json->get_int(key1, key2);
}

int cfg_json_mgr_t::get_int(std::string json_name, int key1, const char* key2) {
	cfg_json_t *json = this->get_json(json_name);
	if (!json) {
		return JSON_ERROR_INT;
	}
	return json->get_int(key1, key2);
}

int cfg_json_mgr_t::get_int(std::string json_name, int key1, int key2) {
	cfg_json_t *json = this->get_json(json_name);
	if (!json) {
		return JSON_ERROR_INT;
	}
	return json->get_int(key1, key2);
}

int cfg_json_mgr_t::get_int(std::string json_name, int key1, const char *key2, int key3) {
	cfg_json_t *json = this->get_json(json_name);
	if (!json) {
		return JSON_ERROR_INT;
	}
	return json->get_int(key1, key2, key3);
}

float cfg_json_mgr_t::get_float(std::string json_name, const char *key1) {
	cfg_json_t *json = this->get_json(json_name);
	if (!json) {
		return JSON_ERROR_FLOAT;
	}
	return json->get_float(key1);
}

float cfg_json_mgr_t::get_float(std::string json_name, int key1) {
	cfg_json_t *json = this->get_json(json_name);
	if (!json) {
		return JSON_ERROR_FLOAT;
	}
	return json->get_float(key1);
}

float cfg_json_mgr_t::get_float(std::string json_name, const char *key1, const char *key2) {
	cfg_json_t *json = this->get_json(json_name);
	if (!json) {
		return JSON_ERROR_FLOAT;
	}
	return json->get_float(key1, key2);
}

float cfg_json_mgr_t::get_float(std::string json_name, const char *key1, int key2) {
	cfg_json_t *json = this->get_json(json_name);
	if (!json) {
		return JSON_ERROR_FLOAT;
	}
	return json->get_float(key1, key2);
}

float cfg_json_mgr_t::get_float(std::string json_name, int key1, const char* key2) {
	cfg_json_t *json = this->get_json(json_name);
	if (!json) {
		return JSON_ERROR_FLOAT;
	}
	return json->get_float(key1, key2);
}

float cfg_json_mgr_t::get_float(std::string json_name, int key1, int key2) {
	cfg_json_t *json = this->get_json(json_name);
	if (!json) {
		return JSON_ERROR_FLOAT;
	}
	return json->get_float(key1, key2);
}

float cfg_json_mgr_t::get_float(std::string json_name, int key1, const char *key2, int key3) {
	cfg_json_t *json = this->get_json(json_name);
	if (!json) {
		return JSON_ERROR_INT;
	}
	return json->get_float(key1, key2, key3);
}

std::string cfg_json_mgr_t::get_string(std::string json_name, const char *key1) {
	cfg_json_t *json = this->get_json(json_name);
	if (!json) {
		return JSON_ERROR_STRING;
	}
	return json->get_string(key1);
}

std::string cfg_json_mgr_t::get_string(std::string json_name, int key1) {
	cfg_json_t *json = this->get_json(json_name);
	if (!json) {
		return JSON_ERROR_STRING;
	}
	return json->get_string(key1);
}

std::string cfg_json_mgr_t::get_string(std::string json_name, const char *key1, const char *key2) {
	cfg_json_t *json = this->get_json(json_name);
	if (!json) {
		return JSON_ERROR_STRING;
	}
	return json->get_string(key1, key2);
}

std::string cfg_json_mgr_t::get_string(std::string json_name, const char *key1, int key2) {
	cfg_json_t *json = this->get_json(json_name);
	if (!json) {
		return JSON_ERROR_STRING;
	}
	return json->get_string(key1, key2);
}

std::string cfg_json_mgr_t::get_string(std::string json_name, int key1, const char* key2) {
	cfg_json_t *json = this->get_json(json_name);
	if (!json) {
		return JSON_ERROR_STRING;
	}
	return json->get_string(key1, key2);
}

std::string cfg_json_mgr_t::get_string(std::string json_name, int key1, int key2) {
	cfg_json_t *json = this->get_json(json_name);
	if (!json) {
		return JSON_ERROR_STRING;
	}
	return json->get_string(key1, key2);
}