#include "../luautils.h"
#include "../renderer\RenderSystem.h"
#include "../Sprite.h"
#include "../Model.h"

static int render_newsprite(lua_State* L) {
	Sprite** sprite = (Sprite **)lua_newuserdata(L, sizeof(Model*));
	*sprite = new Sprite();
	(*sprite)->Init();
	Lua_SetMetatable(L, "Sprite");

	return 1;
}

static int render_newmodel(lua_State* L) {
	Model** model = (Model **)lua_newuserdata(L, sizeof(Model*));
	*model = new Model();
	(*model)->Init();
	Lua_SetMetatable(L, "Model");

	return 1;
}

static int render_addsprite(lua_State* L){
    //RenderSystem* cobj = *reinterpret_cast<RenderSystem**>(lua_touserdata(L, 1));
    lua_getglobal(L, "renderSys");
	RenderSystem* cobj = *reinterpret_cast<RenderSystem**>(lua_touserdata(L, -1));
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'AddSprite'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) {
        Sprite* arg0 = *reinterpret_cast<Sprite**>(lua_touserdata(L, 1));
        bool ret = cobj->AddSprite(arg0);
        lua_pushboolean(L, ret);
        return 1;
    }

    return 0;
}

static const luaL_Reg syslib[] = {
  {"newsprite", render_newsprite},
  {"newmodel", render_newmodel},
  {"addsprite", render_addsprite},

  {NULL, NULL}
};

int luaopen_render(lua_State* L) {
	luaL_register(L, "render", syslib);
    return 1;
}
