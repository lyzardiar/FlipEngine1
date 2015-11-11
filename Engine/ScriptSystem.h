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

	bool Register(const char* name, void* userdata);

	bool Call(const char* funcname);
private:

	lua_State* _state;
};



#endif // !__SCRIPTSYSTEM_H__



