TOLUA_API int register_all_${prefix}(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,nullptr,0);
	tolua_beginmodule(tolua_S,nullptr);

	#for jsclass in $sorted_classes
	#if $in_listed_classes(jsclass)
	lua_register_${prefix}_${jsclass}(tolua_S);
	#end if
	#end for

	tolua_endmodule(tolua_S);
	return 1;
}

#if $macro_judgement
\#endif
#end if
