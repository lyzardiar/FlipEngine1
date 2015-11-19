## ===== instance function implementation template
static int ${namespaced_class_name}${signature_name}(lua_State* L)
{
    ${namespaced_class_name}* cobj = (${namespaced_class_name}*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function '${signature_name}'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
#if len($arguments) >= $min_args
    #set arg_count = len($arguments)
    #set arg_idx = $min_args
    #while $arg_idx <= $arg_count
    if (argc == ${arg_idx}) 
    {
        #set count = 0
        #set arg_list = ""
        #set arg_array = []
        #while count < $arg_idx
            #set arg = $arguments[$count]
        ${arg.to_native($count)}
            #set arg_array += ["arg"+str(count)]
            #set count = count + 1
        #end while
        #set arg_list = ", ".join(arg_array)
        #if $ret_type.name != "void"
            ${ret_type.whole_name} ret = cobj->${func_name}($arg_list);
            ${ret_type.from_native()}
        #else
            cobj->${func_name}($arg_list);
        #end if
    }
        #set $arg_idx = $arg_idx + 1
    #end while
#end if

    return 0;
}

