#include "ScriptSystem.h"
#include "sys/sys_public.h"
#include "common/Str.h"
#include "lua_engine_auto.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};

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

	RegisterAllEngine(_state);
	return true;
}

bool ScriptSystem::RunScript( const char* filePath )
{
	if(luaL_dofile(_state, filePath) != 0)
		Sys_Printf("run scrpit ok");
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
