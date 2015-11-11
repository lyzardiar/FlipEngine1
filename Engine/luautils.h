#ifndef __LUAUTILS_H__
#define __LUAUTILS_H__

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};

void Lua_Function (lua_State* L, const char* name, lua_CFunction func);



#endif


