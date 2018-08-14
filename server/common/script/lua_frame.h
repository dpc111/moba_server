#ifndef LUA_FRAME_H
#define LUA_FRAME_H

#define LUA_MAX_STR_PARAM_LEN 1024

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
};
#include <map>

class tcp_connection_t;
class server_t;

class lua_frame_t {
public:
	typedef std::map<const char *, int> func_map_t;

public:
	lua_frame_t(server_t *server);

	~lua_frame_t();

	lua_State *get_lua_state();

	int load_string(const char *str);

	int load_script(const char *file);

	int run_script();

	static int traceback(lua_State *l);

	bool call_func(const char *funcname, const char *args, ...);

	bool call_func(const char *funcname, const char *args, va_list vlist);

	bool run_func(const char *funcname, const char *fmt, va_list vlist);

	int run_func(const char *funcname, int nargs, int nres, int errfunc);

	bool on_script_func(tcp_connection_t *conn, const char *name);

private:
	server_t *server_;

	func_map_t *func_map_;

	lua_State *lua_state_;

	char str_params_[20][LUA_MAX_STR_PARAM_LEN];
};

#endif
