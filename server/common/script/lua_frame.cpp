#include <tr1/functional>
#include "lua_frame.h"
#include "log.h"
#include "type.h"
#include "tcp_connection.h"
#include "tcp_input_stream.h"
#include "msg_dispatch.h"
#include "tcp_network.h"
#include "server.h"

static const void *aux_topointer(lua_State *l, int idx)
{
	switch(lua_type(l, -idx))
	{
	case LUA_TLIGHTUSERDATA:
		return lua_topointer(l, -idx);
		break;
	case LUA_TUSERDATA:
		return *(void **) lua_touserdata(l, -idx);
		break;
	default:
		return NULL;
	}
}

lua_frame_t::lua_frame_t(server_t *server)
	: server_(server) {
	lua_state_ = luaL_newstate();
	luaL_openlibs(lua_state_);
	server_->get_network()->get_msg_dispatch()->set_on_script_func(std::tr1::bind(&lua_frame_t::on_script_func, this, std::tr1::placeholders::_1, std::tr1::placeholders::_2));
}

lua_frame_t::~lua_frame_t() {
	lua_close(lua_state_);
}

lua_State *lua_frame_t::get_lua_state() {
	return lua_state_;
}

int lua_frame_t::load_string(const char *str) {
	if (luaL_loadstring(lua_state_, str)) {
		ERROR("%s\n", lua_tostring(lua_state_, -1));
		return 1;
	}
	return 0;
}

int lua_frame_t::load_script(const char *file) {
	if (luaL_loadfile(lua_state_, file)) {
		ERROR("load file fail %s\n", lua_tostring(lua_state_, -1));
		return 1;
	}
	return 0;
}

int lua_frame_t::run_script() {
	if (lua_pcall(lua_state_, 0, LUA_MULTRET, 0)) {
		ERROR("%d\n", lua_tostring(lua_state_, -1));
		return 1;
	}
	return 0;
}

int lua_frame_t::traceback(lua_State *l) {
	ERROR(lua_tostring(l, -1));
	return 1;
}

bool lua_frame_t::call_func(const char *funcname, const char *args, ...) {
	va_list vlist;
	va_start(vlist, args);
	bool res = call_func(funcname, args, vlist);
	va_end(vlist);
	return res;
}

bool lua_frame_t::call_func(const char *funcname, const char *args, va_list vlist) {
	int top = lua_gettop(lua_state_);
	bool res = run_func(funcname, args, vlist);
	lua_settop(lua_state_, top);
	return res;
}

bool lua_frame_t::run_func(const char *funcname, const char *fmt, va_list vlist) {
	lua_getglobal(lua_state_, funcname);
	if (lua_isnil(lua_state_, -1)) {
		ERROR("");
		return false;
	}
	lua_pushvalue(lua_state_, -1);
	if (!lua_isfunction(lua_state_, -1)) {
		ERROR("");
		return false;
	}
	lua_pushvalue(lua_state_, -1);
	int nargs = lua_gettop(lua_state_);
	while(*fmt != '\0') {
		switch(*fmt) {
		case 'b' :
			lua_pushboolean(lua_state_, va_arg(vlist, int));
			break;
		case 'i' :
			lua_pushnumber(lua_state_, va_arg(vlist, int));
			break;
		case 'l' :
			lua_pushnumber(lua_state_, (double) va_arg(vlist, int64));
			break;
		case 'd' :
			lua_pushnumber(lua_state_, (double) va_arg(vlist, double));
			break;
		case 's' :
			lua_pushstring(lua_state_, (char *) va_arg(vlist, char *));
			break;
		case 'p' :
			lua_pushlightuserdata(lua_state_, va_arg(vlist, void *));
			break;
		case ':' :
			break;
		default :
			ERROR("");
			return false;
		}
		if (*fmt == ':') {
			++fmt;
			break;
		}
		++fmt;
	}

	nargs = lua_gettop(lua_state_) - nargs;
	int nres = 0;
	const char *walk = fmt;
	while (*walk != '\0') {
		nres += 1;
		++walk;
	}
	lua_pushcfunction(lua_state_, traceback);
	lua_insert(lua_state_, -nargs - 2);
	if (lua_pcall(lua_state_, nargs, nres, -nres - 2) != 0) {
		ERROR("");
		return false;
	}
	while (*fmt != '\0') {
		switch (*fmt) {
		case 'b' :
			*va_arg(vlist, bool *) = lua_toboolean(lua_state_, -nres) != 0;
			break;
		case 'i' :
			*va_arg(vlist, int *) = (int) lua_tonumber(lua_state_, -nres);
			break;
		case 'l' :
			*va_arg(vlist, int64 *) = (int64) lua_tonumber(lua_state_, -nres);
			break;
		case 'd' :
			*va_arg(vlist, double *) = (double) lua_tonumber(lua_state_, -nres);
			break;
		case 's' :
			*va_arg(vlist, const char **) = lua_tostring(lua_state_, -nres);
			break;
		case 'p' :
			*va_arg(vlist, const void **) = aux_topointer(lua_state_, -nres);
			break;
		default :
			ERROR("");
			return false;
		}
		++fmt;
		if (--nres == 0) {
			break;
		}
	}
	return true;
}

int lua_frame_t::run_func(const char *funcname, int nargs, int nres, int errfunc) {
	return lua_pcall(lua_state_, nargs, nres, errfunc);
}

bool lua_frame_t::on_script_func(tcp_connection_t *conn, const char *name) {
	int top = lua_gettop(lua_state_);
	tcp_input_stream_t& stream = conn->get_input_stream();
	ERROR("%s", name);
	lua_getglobal(lua_state_, name);
	if (lua_isnil(lua_state_, 1)) {
		ERROR("");
		lua_settop(lua_state_, top);
		return false;
	}
	lua_pushvalue(lua_state_, -1);
	if (!lua_isfunction(lua_state_, -1)) {
		ERROR("");
		lua_settop(lua_state_, top);
		return false;
	}
	int fpos = lua_gettop(lua_state_);

	int str_param_pos = 0;
	int len;
	stream.read(&len, sizeof(int));
	stream.read(str_params_[str_param_pos], len);
	char *walk = str_params_[str_param_pos];
	str_params_[str_param_pos][len] = '\0';
	while (*walk != '\0') {
		if (*walk == 'i') {
			int val;
			stream.read(&len, sizeof(int));
			stream.read(&val, sizeof(int));
			lua_pushnumber(lua_state_, val);
		} else if (*walk == 'd') {
			double val;
			stream.read(&len, sizeof(int));
			stream.read(&val, sizeof(double));
			lua_pushnumber(lua_state_, val);
		} else if (*walk == 's') {
			++str_param_pos;
			if (str_param_pos >= 20) {
				ERROR("");
				lua_settop(lua_state_, top);
				return false;
			}
			stream.read(&len, sizeof(int));
			stream.read(str_params_[str_param_pos], len);
			lua_pushstring(lua_state_, str_params_[str_param_pos]);
		} else {
			lua_settop(lua_state_, top);
			return false;
		}
		++walk;
	}

	int nargs = lua_gettop(lua_state_) - fpos;
	if (lua_pcall(lua_state_, nargs, 0, 0) != 0) {
		ERROR("");
		lua_settop(lua_state_, top);
		return false;
	}
	lua_settop(lua_state_, top);
	return true;
}
