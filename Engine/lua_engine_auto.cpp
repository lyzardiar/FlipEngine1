#include "lua_engine_auto.h"
#include "../renderer/RenderSystem.h"
#include "../Sprite.h"

static int lnewSprite(lua_State *L) {
	Sprite* obj = new Sprite;
	lua_pushlightuserdata(L, obj);

	/*	luaL_getmetatable(L, "RenderSystem");     
	if (lua_isnil(L, -1)) { 
	lua_pop(L, 1);
	return;
	}
	lua_setmetatable(L,-2); */ 
	return 1;
}

int lRenderSysDrawString(lua_State* tolua_S)
{
	int argc = 0;
	RenderSystem* cobj = nullptr;
	bool ok  = true;

	if (lua_isuserdata(tolua_S, 1))
	{
		cobj = (RenderSystem*)lua_touserdata(tolua_S, 1);
	}

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) 
	{
		const char* str = lua_tostring(tolua_S, 2);
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
		lua_pushcfunction(L, lRenderSysDrawString);
		lua_setfield(L, -2, "DrawString");
	}

	lua_newtable(L);
	lua_pushcfunction(L, lnewSprite);
	lua_setfield(L, -2, "CreateSprite");

	lua_setfield(L, LUA_GLOBALSINDEX, "RenderSystem");

	return 1;
}

