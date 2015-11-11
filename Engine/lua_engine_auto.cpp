#include "lua_engine_auto.h"
#include "../renderer/RenderSystem.h"

class Test
{
public:
	void print() { }
};


int testprint(lua_State* tolua_S)
{
	return 0;
}

static int lnewbuffer(lua_State *L) {
	Test *rb = (Test *)lua_newuserdata(L, sizeof(*rb));
	if (luaL_newmetatable(L, "Test")) {
		lua_pushvalue(L, -1);
		lua_setfield(L, -2, "__index");
		lua_pushcfunction(L, testprint);
		lua_setfield(L, -2, "print");
	}
	lua_setmetatable(L, -2);
	return 1;
}
int lua_register_RenderSystem_DrawString(lua_State* tolua_S)
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

int lua_register_cocos2dx_3d_BillBoard(lua_State* tolua_S)
{
	/*	tolua_usertype(tolua_S,"cc.BillBoard");
	tolua_cclass(tolua_S,"BillBoard","cc.BillBoard","cc.Sprite",nullptr);

	tolua_beginmodule(tolua_S,"BillBoard");
	tolua_function(tolua_S,"getMode", lua_cocos2dx_3d_BillBoard_getMode);
	tolua_function(tolua_S,"setMode", lua_cocos2dx_3d_BillBoard_setMode);
	tolua_function(tolua_S,"create", lua_cocos2dx_3d_BillBoard_create);
	tolua_function(tolua_S,"createWithTexture", lua_cocos2dx_3d_BillBoard_createWithTexture);
	tolua_endmodule(tolua_S)*/;
	return 1;
}

TOLUA_API int register_all_engine(lua_State* tolua_S)
{
	if (luaL_newmetatable(tolua_S, "RenderSystem")) {
		lua_pushvalue(tolua_S, -1);
		lua_setfield(tolua_S, -2, "__index");
		lua_pushcfunction(tolua_S, lua_register_RenderSystem_DrawString);
		lua_setfield(tolua_S, -2, "DrawString");
	}

	return 1;
}

