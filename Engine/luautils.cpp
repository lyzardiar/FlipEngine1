#include "luautils.h"

void Lua_PushFunction (lua_State* L, const char* name, lua_CFunction func)
{
    lua_pushstring(L,name);
    lua_pushcfunction(L,func);
    lua_rawset(L,-3);
}

void Lua_PushCObject(lua_State* L, const char* name, void* userdata)
{
	lua_pushlightuserdata(L, userdata);
	luaL_getmetatable(L, name);                    
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		return;
	}
	lua_setmetatable(L, -2);   
}