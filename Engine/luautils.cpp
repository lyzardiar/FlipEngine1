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

void Lua_PushVec3( lua_State* L, float x, float y, float z )
{
	lua_createtable(L, 0, 0);
	lua_pushnumber(L, x);
	lua_setfield(L, -2, "x");
	lua_pushnumber(L, y);
	lua_setfield(L, -2, "y");
	lua_pushnumber(L, z);
	lua_setfield(L, -2, "z");
}

void Lua_PushVec2( lua_State* L, float x, float y )
{
	lua_createtable(L, 0, 0);
	lua_pushnumber(L, x);
	lua_setfield(L, -2, "x");
	lua_pushnumber(L, y);
	lua_setfield(L, -2, "y");
}


int register_all_auto(lua_State* L)
{
	lua_register_Sprite(L);
	lua_register_RenderSystem(L);
	return 1;
}