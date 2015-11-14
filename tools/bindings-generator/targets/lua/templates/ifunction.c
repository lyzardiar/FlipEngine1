## ===== instance function implementation template
int ${signature_name}(lua_State* L)
{
    ${namespaced_class_name}* cobj = (${namespaced_class_name}*)lua_touserdata(L,1,0);;
    if (!cobj) 
    {
        tolua_error(L,"invalid 'cobj' in function '${signature_name}'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
#if len($arguments) >= $min_args
    #set arg_count = len($arguments)
    #set arg_idx = $min_args
    #while $arg_idx <= $arg_count
    if (argc == ${arg_idx}) 
    {
        #set $count = 0
        #while $count < $arg_idx
            #set $arg = $arguments[$count]
        ${arg.to_string($generator)} arg${count};
            #set $count = $count + 1
        #end while
        #set $count = 0
        #set arg_list = ""
        #set arg_array = []
        #while $count < $arg_idx
            #set $arg = $arguments[$count]

        ${arg.to_native({"generator": $generator,
                                      "in_value": "argv[" + str(count) + "]",
                                      "out_value": "arg" + str(count),
                                      "arg_idx": $count+2,
                                      "class_name": $class_name,
                                      "lua_namespaced_class_name": $generator.scriptname_from_native($namespaced_class_name, $namespace_name),
                                      "func_name": $func_name,
                                      "level": 2,
                                      "arg":$arg,
                                      "ntype": $arg.namespaced_name.replace("*", ""),
                                      "scriptname": $generator.scriptname_from_native($arg.namespaced_name, $arg.namespace_name)})};
            #set $arg_array += ["arg"+str(count)]
            #set $count = $count + 1
        #end while

        #set $arg_list = ", ".join($arg_array)
        cobj->${func_name}($arg_list);
        lua_settop(L, 1);
        return 1;
                #end if
        #end if         
    }
        #set $arg_idx = $arg_idx + 1
    #end while
#end if
    luaL_error(L, "%s has wrong number of arguments: %d, was expecting %d \n", "${generator.scriptname_from_native($namespaced_class_name, $namespace_name)}:${func_name}",argc, ${min_args});

    return 0;
}
