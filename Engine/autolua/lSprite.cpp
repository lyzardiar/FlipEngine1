#include "../luautils.h"
#include "../Sprite.h"

static int SpriteSetLabel(lua_State* L){
    Sprite* cobj = (Sprite*)lua_touserdata(L,1);
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'SetLabel'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) {
        const char* arg0 = lua_tostring(L, 2);
        cobj->SetLabel(arg0);
    }

    return 0;
}

static int SpriteSetTexture(lua_State* L){
    Sprite* cobj = (Sprite*)lua_touserdata(L,1);
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'SetTexture'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) {
        const char* arg0 = lua_tostring(L, 2);
        cobj->SetTexture(arg0);
    }

    return 0;
}

static int SpriteSetPosition(lua_State* L){
    Sprite* cobj = (Sprite*)lua_touserdata(L,1);
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'SetPosition'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 3) {
        float arg0 = lua_tonumber(L, 2);
        float arg1 = lua_tonumber(L, 3);
        float arg2 = lua_tonumber(L, 4);
        cobj->SetPosition(arg0, arg1, arg2);
    }

    return 0;
}

static int SpriteGetPosition(lua_State* L){
    Sprite* cobj = (Sprite*)lua_touserdata(L,1);
    if (!cobj) {
        luaL_error(L,"invalid 'cobj' in function 'GetPosition'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) {
        vec3 ret = cobj->GetPosition();
        Lua_PushVec3(L, ret.x, ret.y, ret.z);
        return 1;
    }

    return 0;
}


int lua_register_Sprite(lua_State* L)
{
    if (luaL_newmetatable(L, "Sprite")) {
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        Lua_PushFunction(L, "setLabel", SpriteSetLabel);
        Lua_PushFunction(L, "setTexture", SpriteSetTexture);
        Lua_PushFunction(L, "setPosition", SpriteSetPosition);
        Lua_PushFunction(L, "getPosition", SpriteGetPosition);
    }
    return 1;
}
