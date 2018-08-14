#include <fstream> 
#include "cfg_json.h"
#include "log.h"

cfg_json_t::cfg_json_t(cfg_json_mgr_t *mgr) {
	mgr_ = mgr;
	file_name_ = NULL;
}

cfg_json_t::~cfg_json_t() {
	mgr_ = NULL;
	file_name_ = NULL;
}

bool cfg_json_t::init(const char *file_name) {
	file_name_ = file_name;
	std::ifstream ifs(file_name);
	if (!ifs.is_open()) {
		ERROR("open failed %s", file_name);
		return false;
	}
	if (!mgr_->get_json_reader()->parse(ifs, json_value_))
	{
		ERROR("parse failed %s", file_name);
		ifs.close();
		return false;
	}
	ifs.close();
	return true;
}

bool cfg_json_t::exist(int key1) {
	if (!json_value_.isArray()) {
		return false;
	}
	Json::Value value = json_value_[key1];
	if (value.isNull()) {
		return false;
	}
	return true;
}

int cfg_json_t::get_int(const char *key1) {
	if (!json_value_.isObject()) { 
		ERROR("");
		return JSON_ERROR_INT;
	}
	Json::Value value = json_value_[key1];
	if (value.isNull() || 
		(!value.isInt() &&
		!value.isUInt())) {
		ERROR("");
		return JSON_ERROR_INT;
	}
	return value.asInt();
}

int cfg_json_t::get_int(int key1) {
	if (!json_value_.isArray()) { 
		ERROR("");
		return JSON_ERROR_INT;
	}
	Json::Value value = json_value_[key1];
	if (value.isNull() || 
		(!value.isInt() &&
		!value.isUInt())) {
		ERROR("");
		return JSON_ERROR_INT;
	}
	return value.asInt();
}

int cfg_json_t::get_int(const char *key1, const char *key2) {
	if (!json_value_.isObject() ||
		!json_value_[key1].isObject()) { 
		ERROR("");
		return JSON_ERROR_INT;
	}
	Json::Value value = json_value_[key1][key2];
	if (value.isNull() || 
		(!value.isInt() &&
		!value.isUInt())) {
		ERROR("");
		return JSON_ERROR_INT;
	}
	return value.asInt();
}

int cfg_json_t::get_int(const char *key1, int key2) {
	if (!json_value_.isObject() ||
		!json_value_[key1].isArray()) { 
		ERROR("");
		return JSON_ERROR_INT;
	}
	Json::Value value = json_value_[key1][key2];
	if (value.isNull() || 
		(!value.isInt() &&
		!value.isUInt())) {
		ERROR("");
		return JSON_ERROR_INT;
	}
	return value.asInt();
}

int cfg_json_t::get_int(int key1, const char* key2) {
	if (!json_value_.isArray() ||
		!json_value_[key1].isObject()) { 
		ERROR("");
		return JSON_ERROR_INT;
	}
	Json::Value value = json_value_[key1][key2];
	if (value.isNull() || 
		(!value.isInt() &&
		!value.isUInt())) {
		ERROR("");
		return JSON_ERROR_INT;
	}
	return value.asInt();
}

int cfg_json_t::get_int(int key1, int key2) {
	if (!json_value_.isArray() ||
		!json_value_[key1].isArray()) { 
		ERROR("");
		return JSON_ERROR_INT;
	}
	Json::Value value = json_value_[key1][key2];
	if (value.isNull() || 
		(!value.isInt() &&
		!value.isUInt())) {
		ERROR("");
		return JSON_ERROR_INT;
	}
	return value.asInt();
}

int cfg_json_t::get_int(int key1, const char *key2, int key3) {
	if (!json_value_.isArray() ||
		!json_value_[key1].isObject() ||
		!json_value_[key1][key2].isArray()) {
		ERROR("");
		return JSON_ERROR_INT;
	}
	Json::Value value = json_value_[key1][key2][key3];
	if (value.isNumeric()) {
		ERROR("");
		return JSON_ERROR_INT;
	}
	return value.asInt();
}

float cfg_json_t::get_float(const char *key1) {
	if (!json_value_.isObject()) {
		ERROR("");
		return JSON_ERROR_FLOAT;
	}
	Json::Value value = json_value_[key1];
	// if (!value.isDouble()) {
	// 	ERROR("");
	// 	return JSON_ERROR_FLOAT;
	// }
	return (float)value.asDouble();

}

float cfg_json_t::get_float(int key1) {
	if (!json_value_.isArray()) {
		ERROR("");
		return JSON_ERROR_FLOAT;
	}
	Json::Value value = json_value_[key1];
	// if (!value.isDouble()) {
	// 	ERROR("");
	// 	return JSON_ERROR_FLOAT;
	// }
	return (float)value.asDouble();
}

float cfg_json_t::get_float(const char *key1, const char *key2) {
	if (!json_value_.isObject() ||
		!json_value_[key1].isObject()) {
		ERROR("");
		return JSON_ERROR_FLOAT;
	}
	Json::Value value = json_value_[key1][key2];
	// if (!value.isDouble()) {
	// 	ERROR("");
	// 	return JSON_ERROR_FLOAT;
	// }
	return (float)value.asDouble();
}

float cfg_json_t::get_float(const char *key1, int key2) {
	if (!json_value_.isObject() ||
		!json_value_[key1].isArray()) {
		ERROR("");
		return JSON_ERROR_FLOAT;
	}
	Json::Value value = json_value_[key1][key2];
	// if (!value.isDouble()) {
	// 	ERROR("");
	// 	return JSON_ERROR_FLOAT;
	// }
	return (float)value.asDouble();
}

float cfg_json_t::get_float(int key1, const char* key2) {
	if (!json_value_.isArray() ||
		!json_value_[key1].isObject()) {
		ERROR("");
		return JSON_ERROR_FLOAT;
	}
	Json::Value value = json_value_[key1][key2];
	// if (!value.isDouble()) {
	// 	ERROR("");
	// 	return JSON_ERROR_FLOAT;
	// }
	return (float)value.asDouble();
}

float cfg_json_t::get_float(int key1, int key2) {
	if (!json_value_.isArray() ||
		!json_value_[key1].isArray()) {
		ERROR("");
		return JSON_ERROR_FLOAT;
	}
	Json::Value value = json_value_[key1][key2];
	// if (!value.isDouble()) {
	// 	ERROR("");
	// 	return JSON_ERROR_FLOAT;
	// }
	return (float)value.asDouble();
}

float cfg_json_t::get_float(int key1, const char *key2, int key3) {
	if (!json_value_.isArray() ||
		!json_value_[key1].isObject() ||
		!json_value_[key1][key2].isArray()) {
		ERROR("");
		return JSON_ERROR_FLOAT;
	}
	Json::Value value = json_value_[key1][key2][key3];
	// if (value.isNumeric()) {
	// 	ERROR("");
	// 	return JSON_ERROR_FLOAT;
	// }
	return value.asDouble();
}

std::string cfg_json_t::get_string(const char *key1) {
	if (!json_value_.isObject()) { 
		return JSON_ERROR_STRING;
	}
	Json::Value value = json_value_[key1];
	if (value.isNull() || 
		!value.isString()) {
		return JSON_ERROR_STRING;
	}
	return value.asString();
}

std::string cfg_json_t::get_string(int key1) {
	if (!json_value_.isArray()) { 
		return JSON_ERROR_STRING;
	}
	Json::Value value = json_value_[key1];
	if (value.isNull() || 
		!value.isString()) {
		return JSON_ERROR_STRING;
	}
	return value.asString();
}

std::string cfg_json_t::get_string(const char *key1, const char *key2) {
	if (!json_value_.isObject() ||
		!json_value_[key1].isObject()) { 
		return JSON_ERROR_STRING;
	}
	Json::Value value = json_value_[key1][key2];
	if (value.isNull() || 
		!value.isString()) {
		return JSON_ERROR_STRING;
	}
	return value.asString();
}

std::string cfg_json_t::get_string(const char *key1, int key2) {
	if (!json_value_.isObject() ||
		!json_value_[key1].isArray()) { 
		return JSON_ERROR_STRING;
	}
	Json::Value value = json_value_[key1][key2];
	if (value.isNull() || 
		!value.isString()) {
		return JSON_ERROR_STRING;
	}
	return value.asString();
}

std::string cfg_json_t::get_string(int key1, const char* key2) {
	if (!json_value_.isArray() ||
		!json_value_[key1].isObject()) { 
		return JSON_ERROR_STRING;
	}
	Json::Value value = json_value_[key1][key2];
	if (value.isNull() || 
		!value.isString()) {
		return JSON_ERROR_STRING;
	}
	return value.asString();
}

std::string cfg_json_t::get_string(int key1, int key2) {
	if (!json_value_.isArray() ||
		!json_value_[key1].isArray()) { 
		return JSON_ERROR_STRING;
	}
	Json::Value value = json_value_[key1][key2];
	if (value.isNull() || 
		!value.isString()) {
		return JSON_ERROR_STRING;
	}
	return value.asString();
}