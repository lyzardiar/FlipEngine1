#ifndef __LUAUTILS_H__
#define __LUAUTILS_H__

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};

void Lua_PushFunction (lua_State* L, const char* name, lua_CFunction func);

void Lua_PushCObject(lua_State* L, const char* name, void* userdata);

#endif


