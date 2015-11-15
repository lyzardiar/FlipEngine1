
int lua_register_${class_name}(lua_State* L)
{
    if (luaL_newmetatable(L, ${class_name})) {
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
${writemethods()}
    }
    return 1;
}
