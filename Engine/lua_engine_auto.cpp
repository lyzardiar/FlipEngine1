#include "luautils.h"
#include "lua_engine_auto.h"
#include "../renderer/RenderSystem.h"
#include "../Sprite.h"

static int lnewSprite(lua_State *L) {
	Sprite* obj = new Sprite;
	Lua_PushCObject(L, "Sprite", obj);
	return 1;
}

static int lSpriteSetTexture(lua_State* L)
{
	Sprite* cobj = (Sprite*)lua_touserdata(L, 1);
	int argc = lua_gettop(L)-1;
	if (argc == 1) 
	{
		const char* str = lua_tostring(L, 2);
		cobj->SetTexture(str);
		return 1;
	}
	return 0;
}

static int lSpriteSetLabel(lua_State* L)
{
	Sprite* cobj = (Sprite*)lua_touserdata(L, 1);
	int argc = lua_gettop(L)-1;
	if (argc == 1) 
	{
		const char* str = lua_tostring(L, 2);
		cobj->SetLabel(str);
		return 1;
	}
	luaL_error(L, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.Sprite3D:setCullFace",argc, 1);
	return 0;
}

static int lSpriteSetPosition(lua_State* L)
{
	Sprite* cobj = (Sprite*)lua_touserdata(L, 1);
	int argc = lua_gettop(L)-1;
	if (argc == 3) 
	{
		double x = lua_tonumber(L, 2);
		double y = lua_tonumber(L, 3);
		double z = lua_tonumber(L, 4);
		cobj->SetPosition(x, y, z);
		return 1;
	}
	luaL_error(L, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.Sprite3D:setCullFace",argc, 1);
	return 0;
}


int lRenderSysDrawString(lua_State* L)
{
	int argc = 0;
	RenderSystem* cobj = nullptr;
	bool ok  = true;

	if (lua_isuserdata(L, 1))
	{
		cobj = (RenderSystem*)lua_touserdata(L, 1);
	}

	argc = lua_gettop(L)-1;
	if (argc == 1) 
	{
		const char* str = lua_tostring(L, 2);
		cobj->DrawString(str);
		return 1;
	}
	//luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.Sprite3D:setCullFace",argc, 1);
	return 0;
}

int RegisterAllEngine(lua_State* L)
{
	if (luaL_newmetatable(L, "RenderSystem")) {
		lua_pushvalue(L, -1);
		lua_setfield(L, -2, "__index");
		Lua_PushFunction(L, "DrawString", lRenderSysDrawString);
	}

	if (luaL_newmetatable(L, "Sprite")) {
		lua_pushvalue(L, -1);
		lua_setfield(L, -2, "__index");
		Lua_PushFunction(L, "SetTexture", lSpriteSetTexture);
		Lua_PushFunction(L, "SetLabel", lSpriteSetLabel);
		Lua_PushFunction(L, "SetPosition", lSpriteSetPosition);
	}

	if (luaL_newmetatable(L, "Model")) {
		lua_pushvalue(L, -1);
		lua_setfield(L, -2, "__index");
		Lua_PushFunction(L, "SetTexture", lSpriteSetTexture);
		Lua_PushFunction(L, "SetLabel", lSpriteSetLabel);
		Lua_PushFunction(L, "SetPosition", lSpriteSetPosition);
	}

	lua_newtable(L);
	Lua_PushFunction(L, "CreateSprite", lnewSprite);

	lua_setfield(L, LUA_GLOBALSINDEX, "RS");

	return 1;
}

