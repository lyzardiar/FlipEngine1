#include "ScriptSystem.h"
#include "sys/sys_public.h"
#include "common/Str.h"

#include "luautils.h"

#include "Sprite.h"

int lua_print(lua_State * luastate)
{
	int nargs = lua_gettop(luastate);

	lfStr t;
	for (int i=1; i <= nargs; i++)
	{
		if (lua_istable(luastate, i))
			t += "table";
		else if (lua_isnone(luastate, i))
			t += "none";
		else if (lua_isnil(luastate, i))
			t += "nil";
		else if (lua_isboolean(luastate, i))
		{
			if (lua_toboolean(luastate, i) != 0)
				t += "true";
			else
				t += "false";
		}
		else if (lua_isfunction(luastate, i))
			t += "function";
		else if (lua_islightuserdata(luastate, i))
			t += "lightuserdata";
		else if (lua_isthread(luastate, i))
			t += "thread";
		else
		{
			const char * str = lua_tostring(luastate, i);
			if (str)
				t += lua_tostring(luastate, i);
			else
				t += lua_typename(luastate, lua_type(luastate, i));
		}
		if (i!=nargs)
			t += "\t";
	}
	Sys_Printf("%s\n", t.c_str());

	return 0;
}

static int newSprite(lua_State* L)
{
	Sprite* sprite = (Sprite *)lua_newuserdata(L, sizeof(*sprite));
	sprite->Init();
	luaL_getmetatable(L, "Sprite");                    
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		return 0;
	}

	lua_setmetatable(L, -2);  
	//Lua_PushCObject(L, "Sprite", sprite);
	return 1;
}



ScriptSystem::ScriptSystem()
{
}

ScriptSystem::~ScriptSystem()
{
}

bool ScriptSystem::Init()
{
	_state = lua_open();
	luaL_openlibs(_state);


	// Register our version of the global "print" function
	const luaL_reg global_functions [] = {
		{"print", lua_print},
		{nullptr, nullptr}
	};
	luaL_register(_state, "_G", global_functions);

	register_all_auto(_state);

	lua_createtable(_state, 0, 0);
	lua_setfield(_state, LUA_GLOBALSINDEX, "RS");
	lua_pop(_state, 1);

	lua_pushstring(_state, "RS");
	lua_getfield(_state, LUA_GLOBALSINDEX, "RS");

	if(lua_istable(_state, -1))
		Lua_PushFunction(_state, "newSprite", newSprite);

	lua_pop(_state, 1);

	return true;
}

bool ScriptSystem::RunScript( const char* filePath )
{
	int err = luaL_dofile(_state, filePath);
	if(err != 0)
	{
		const char* s = lua_tostring(_state, -1);
		Sys_Error("script system call %s error code %d\n %s", filePath, err, s);
	}

	return true;
}


bool ScriptSystem::Call( const char* funcname )
{
	lua_getglobal(_state, funcname);
	int err = lua_pcall(_state, 0, 0, 0);
	if (err != 0)
	{
		Sys_Error("script system call %s error code %d", funcname, err);
	}
	return false;
}

lua_State* ScriptSystem::GetLuaState()
{
	return _state;
}
