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

void Lua_PushVec3(lua_State* L, float x, float y, float z);

void Lua_PushVec2(lua_State* L, float x, float y);


int register_all_auto(lua_State* L);

// ---------------------------------------------------------------------------
int lua_register_Sprite(lua_State* L);

int lua_register_RenderSystem(lua_State* L);



#endif


