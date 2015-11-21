#include "../luautils.h"
#include "../renderer\RenderSystem.h"

static int RenderSystemAddDrawSur(lua_State* L){
    RenderSystem* cobj = (RenderSystem*)lua_touserdata(L,1);
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'AddDrawSur'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) {
        drawSurf_t* arg0 = (drawSurf_t*)lua_touserdata(L, 2);
        bool ret = cobj->AddDrawSur(arg0);
        lua_pushboolean(L, ret);
        return 1;
    }

    return 0;
}

static int RenderSystemAddStaticModel(lua_State* L){
    RenderSystem* cobj = (RenderSystem*)lua_touserdata(L,1);
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'AddStaticModel'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) {
        StaticModel* arg0 = (StaticModel*)lua_touserdata(L, 2);
        bool ret = cobj->AddStaticModel(arg0);
        lua_pushboolean(L, ret);
        return 1;
    }

    return 0;
}

static int RenderSystemAddSprite(lua_State* L){
    RenderSystem* cobj = (RenderSystem*)lua_touserdata(L,1);
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'AddSprite'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) {
        Sprite* arg0 = (Sprite*)lua_touserdata(L, 2);
        bool ret = cobj->AddSprite(arg0);
        lua_pushboolean(L, ret);
        return 1;
    }

    return 0;
}

static int RenderSystemDrawString(lua_State* L){
    RenderSystem* cobj = (RenderSystem*)lua_touserdata(L,1);
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'DrawString'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) {
        const char* arg0 = lua_tostring(L, 2);
        cobj->DrawString(arg0);
    }

    return 0;
}


int lua_register_RenderSystem(lua_State* L)
{
    if (luaL_newmetatable(L, "RenderSystem")) {
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        Lua_PushFunction(L, "addDrawSur", RenderSystemAddDrawSur);
        Lua_PushFunction(L, "addStaticModel", RenderSystemAddStaticModel);
        Lua_PushFunction(L, "addSprite", RenderSystemAddSprite);
        Lua_PushFunction(L, "drawString", RenderSystemDrawString);
    }
    return 1;
}
