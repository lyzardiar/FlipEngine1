int register_all_auto(lua_State* L)
{
	#for nclass in $classlist
	lua_register_${nclass}(L);
	#end for
}
