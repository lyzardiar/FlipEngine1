#ifndef __SCRIPTSYSTEM_H__
#define __SCRIPTSYSTEM_H__

struct lua_State;

class ScriptSystem
{
public:
	ScriptSystem();
	~ScriptSystem();

	bool Init();

	bool RunScript(const char* filePath);


	bool Call(const char* funcname);

	lua_State* GetLuaState();
private:

	lua_State* _state;
};



#endif // !__SCRIPTSYSTEM_H__



