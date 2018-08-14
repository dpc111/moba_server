#ifndef CFG_JSON_MGR_H
#define CFG_JSON_MGR_H

#define JSON_ERROR_INT 0xFFFFFFFF
#define JSON_ERROR_FLOAT 0
#define JSON_ERROR_STRING "json_error_string"

#include "stdafx.h"
#include "json/json.h"

class cfg_json_t;

class cfg_json_mgr_t {
public:
	typedef std::map<std::string, cfg_json_t *> cfg_json_map_t;

public:
	cfg_json_mgr_t();

	~cfg_json_mgr_t();

	void load(std::string file_name, std::string json_name);

	Json::Reader* get_json_reader() { return reader_; }

	cfg_json_t* get_json(std::string json_name);

	bool exist(std::string json_name, int32 key1);

	int get_int(std::string json_name, const char *key1);

	int get_int(std::string json_name, int key1);

	int get_int(std::string json_name, const char *key1, const char *key2);

	int get_int(std::string json_name, const char *key1, int key2);

	int get_int(std::string json_name, int key1, const char* key2);

	int get_int(std::string json_name, int key1, int key2);

	int get_int(std::string json_name, int key1, const char *key2, int key3);

	float get_float(std::string json_name, const char *key1);

	float get_float(std::string json_name, int key1);

	float get_float(std::string json_name, const char *key1, const char *key2);

	float get_float(std::string json_name, const char *key1, int key2);

	float get_float(std::string json_name, int key1, const char* key2);

	float get_float(std::string json_name, int key1, int key2);

	float get_float(std::string json_name, int key1, const char *key2, int key3);

	std::string get_string(std::string json_name, const char *key1);

	std::string get_string(std::string json_name, int key1);

	std::string get_string(std::string json_name, const char *key1, const char *key2);

	std::string get_string(std::string json_name, const char *key1, int key2);

	std::string get_string(std::string json_name, int key1, const char* key2);

	std::string get_string(std::string json_name, int key1, int key2);

private:
	Json::Reader *reader_;

	cfg_json_map_t jsons_;
};

#endif