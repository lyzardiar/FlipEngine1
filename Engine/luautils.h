#ifndef __LUAUTILS_H__
#define __LUAUTILS_H__

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};

void Lua_PushFunction (lua_State* L, const char* name, lua_CFunction func);

void Lua_SetMetatable( lua_State* L, const char* metaname );

void Lua_PushVec3(lua_State* L, float x, float y, float z);

void Lua_PushVec2(lua_State* L, float x, float y);


// ---------------------------------------------------------------------------
int luaopen_sprite(lua_State* L);

int luaopen_rendersystem(lua_State* L);

int luaopen_render(lua_State* L);

#endif


