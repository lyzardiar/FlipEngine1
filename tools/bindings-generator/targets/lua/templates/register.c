
int luaopen_${class_name.lower()}(lua_State* L)
{
    if (luaL_newmetatable(L, "${class_name}")) {
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        #for m in $methods_clean()
        #set fn = m['impl']
        Lua_PushFunction(L, "${m['lname']}", ${class_name.lower()}_$(fn.signature_name.lower()));
    	#end for
    }
    return 1;
}
