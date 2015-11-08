#include "lua_engine_auto.h"
#include "ResourceSystem.h"

int lua_engine_resourcesystem_addMesh(lua_State* tolua_S)
{
	/*int argc = 0;
	ResourceSystem* cobj = nullptr;
	bool ok  = true;

	cobj = (ResourceSystem*)tolua_tousertype(tolua_S,1,0);

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) 
	{
		unsigned int arg0;

		ok &= luaval_to_uint32(tolua_S, 2,&arg0, "cc.Sprite3D:setCullFace");
		if(!ok)
		{
			tolua_error(tolua_S,"invalid arguments in function 'lua_cocos2dx_3d_Sprite3D_setCullFace'", nullptr);
			return 0;
		}
		cobj->AddMesh(arg0);
		lua_settop(tolua_S, 1);
		return 1;
	}*/
	//luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.Sprite3D:setCullFace",argc, 1);
	return 0;
}

//int lua_register_cocos2dx_3d_BillBoard(lua_State* tolua_S)
//{
//	tolua_usertype(tolua_S,"cc.BillBoard");
//	tolua_cclass(tolua_S,"BillBoard","cc.BillBoard","cc.Sprite",nullptr);
//
//	tolua_beginmodule(tolua_S,"BillBoard");
//	tolua_function(tolua_S,"getMode",lua_cocos2dx_3d_BillBoard_getMode);
//	tolua_function(tolua_S,"setMode",lua_cocos2dx_3d_BillBoard_setMode);
//	tolua_function(tolua_S,"create", lua_cocos2dx_3d_BillBoard_create);
//	tolua_function(tolua_S,"createWithTexture", lua_cocos2dx_3d_BillBoard_createWithTexture);
//	tolua_endmodule(tolua_S);
//	return 1;
//}
TOLUA_API int register_all_engine(lua_State* tolua_S)
{
	tolua_open(tolua_S);

	tolua_module(tolua_S, "ResourceSystem", 0);
	tolua_beginmodule(tolua_S, "ResourceSystem");
	
	tolua_function(tolua_S, "AddMesh", lua_engine_resourcesystem_addMesh);

	tolua_endmodule(tolua_S);
	return 1;
}

