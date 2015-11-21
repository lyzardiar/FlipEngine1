from clang import cindex
import sys
import pdb
import ConfigParser
import yaml
import re
import os
import inspect
import traceback
from Cheetah.Template import Template
from nativenamespace import *

type_map = {
    cindex.TypeKind.VOID        : "void",
    cindex.TypeKind.BOOL        : "bool",
    cindex.TypeKind.CHAR_U      : "unsigned char",
    cindex.TypeKind.UCHAR       : "unsigned char",
    cindex.TypeKind.CHAR16      : "char",
    cindex.TypeKind.CHAR32      : "char",
    cindex.TypeKind.USHORT      : "unsigned short",
    cindex.TypeKind.UINT        : "unsigned int",
    cindex.TypeKind.ULONG       : "unsigned long",
    cindex.TypeKind.ULONGLONG   : "unsigned long long",
    cindex.TypeKind.CHAR_S      : "char",
    cindex.TypeKind.SCHAR       : "char",
    cindex.TypeKind.WCHAR       : "wchar_t",
    cindex.TypeKind.SHORT       : "short",
    cindex.TypeKind.INT         : "int",
    cindex.TypeKind.LONG        : "long",
    cindex.TypeKind.LONGLONG    : "long long",
    cindex.TypeKind.FLOAT       : "float",
    cindex.TypeKind.DOUBLE      : "double",
    cindex.TypeKind.LONGDOUBLE  : "long double",
    cindex.TypeKind.NULLPTR     : "NULL",
    cindex.TypeKind.OBJCID      : "id",
    cindex.TypeKind.OBJCCLASS   : "class",
    cindex.TypeKind.OBJCSEL     : "SEL",
    # cindex.TypeKind.ENUM        : "int"
}

def native_name_from_type(ntype, underlying=False):
    kind = ntype.kind #get_canonical().kind
    const = "" #"const " if ntype.is_const_qualified() else ""
    if not underlying and kind == cindex.TypeKind.ENUM:
        decl = ntype.get_declaration()
        return get_namespaced_name(decl)
    elif kind in type_map:
        return const + type_map[kind]
    elif kind == cindex.TypeKind.RECORD:
        # might be an std::string
        decl = ntype.get_declaration()
        parent = decl.semantic_parent
        cdecl = ntype.get_canonical().get_declaration()
        cparent = cdecl.semantic_parent
        if decl.spelling == "string" and parent and parent.spelling == "std":
            return "std::string"
        elif cdecl.spelling == "function" and cparent and cparent.spelling == "std":
            return "std::function"
        else:
            # print >> sys.stderr, "probably a function pointer: " + str(decl.spelling)
            return const + decl.spelling
    else:
        # name = ntype.get_declaration().spelling
        # print >> sys.stderr, "Unknown type: " + str(kind) + " " + str(name)
        return INVALID_NATIVE_TYPE
        # pdb.set_trace()

class NativeType(object):
    def __init__(self):
        self.is_object = False
        self.is_function = False
        self.is_enum = False
        self.not_supported = False
        self.param_types = []
        self.ret_type = None
        self.namespaced_name = ""
        self.namespace_name  = ""
        self.name = ""
        self.whole_name = None
        self.is_const = False
        self.is_pointer = False
        self.canonical_type = None

    @staticmethod
    def from_type(ntype):
        if ntype.kind == cindex.TypeKind.POINTER:
            nt = NativeType.from_type(ntype.get_pointee())

            if None != nt.canonical_type:
                nt.canonical_type.name += "*"
                nt.canonical_type.namespaced_name += "*"
                nt.canonical_type.whole_name += "*"

            nt.name += "*"
            nt.namespaced_name += "*"
            nt.whole_name = nt.namespaced_name
            nt.is_enum = False
            nt.is_const = ntype.get_pointee().is_const_qualified()
            nt.is_pointer = True
            if nt.is_const:
                nt.whole_name = "const " + nt.whole_name
        elif ntype.kind == cindex.TypeKind.LVALUEREFERENCE:
            nt = NativeType.from_type(ntype.get_pointee())
            nt.is_const = ntype.get_pointee().is_const_qualified()
            nt.whole_name = nt.namespaced_name + "&"

            if nt.is_const:
                nt.whole_name = "const " + nt.whole_name

            if None != nt.canonical_type:
                nt.canonical_type.whole_name += "&"
        else:
            nt = NativeType()
            decl = ntype.get_declaration()

            if ntype.kind == cindex.TypeKind.RECORD:
                if decl.kind == cindex.CursorKind.CLASS_DECL:
                    nt.is_object = True
                nt.name = decl.displayname
                nt.namespaced_name = get_namespaced_name(decl)
                nt.namespace_name  = get_namespace_name(decl)
                nt.whole_name = nt.namespaced_name
            else:
                if decl.kind == cindex.CursorKind.NO_DECL_FOUND:
                    nt.name = native_name_from_type(ntype)
                else:
                    nt.name = decl.spelling
                nt.namespaced_name = get_namespaced_name(decl)
                nt.namespace_name  = get_namespace_name(decl)

                if nt.namespaced_name == "std::string":
                    nt.name = nt.namespaced_name

                if nt.namespaced_name.startswith("std::function"):
                    nt.name = "std::function"

                if len(nt.namespaced_name) == 0 or nt.namespaced_name.find("::") == -1:
                    nt.namespaced_name = nt.name

                nt.whole_name = nt.namespaced_name
                nt.is_const = ntype.is_const_qualified()
                if nt.is_const:
                    nt.whole_name = "const " + nt.whole_name

                # Check whether it's a std::function typedef
                cdecl = ntype.get_canonical().get_declaration()
                if None != cdecl.spelling and 0 == cmp(cdecl.spelling, "function"):
                    nt.name = "std::function"

                if nt.name != INVALID_NATIVE_TYPE and nt.name != "std::string" and nt.name != "std::function":
                    if ntype.kind == cindex.TypeKind.UNEXPOSED or ntype.kind == cindex.TypeKind.TYPEDEF:
                        ret = NativeType.from_type(ntype.get_canonical())
                        if ret.name != "":
                            if decl.kind == cindex.CursorKind.TYPEDEF_DECL:
                                ret.canonical_type = nt
                            return ret

                nt.is_enum = ntype.get_canonical().kind == cindex.TypeKind.ENUM

                if nt.name == "std::function":
                    nt.namespaced_name = get_namespaced_name(cdecl)                    
                    r = re.compile('function<(.+) .*\((.*)\)>').search(cdecl.displayname)
                    (ret_type, params) = r.groups()
                    params = filter(None, params.split(", "))

                    nt.is_function = True
                    nt.ret_type = NativeType.from_string(ret_type)
                    nt.param_types = [NativeType.from_string(string) for string in params]

        # mark argument as not supported
        if nt.name == INVALID_NATIVE_TYPE:
            nt.not_supported = True

        return nt

    @staticmethod
    def from_string(displayname):
        displayname = displayname.replace(" *", "*")

        nt = NativeType()
        nt.name = displayname.split("::")[-1]
        nt.namespaced_name = displayname
        nt.whole_name = nt.namespaced_name
        nt.is_object = True
        return nt

    @property
    def lambda_parameters(self):
        params = ["%s larg%d" % (str(nt), i) for i, nt in enumerate(self.param_types)]
        return ", ".join(params)

    @staticmethod
    def dict_has_key_re(dict, real_key_list):
        for real_key in real_key_list:
            for (k, v) in dict.items():
                if k.startswith('@'):
                    k = k[1:]
                    match = re.match("^" + k + "$", real_key)
                    if match:
                        return True
                else:
                    if k == real_key:
                        return True
        return False

    @staticmethod
    def dict_get_value_re(dict, real_key_list):
        for real_key in real_key_list:
            for (k, v) in dict.items():
                if k.startswith('@'):
                    k = k[1:]
                    match = re.match("^" + k + "$", real_key)
                    if match:
                        return v
                else:
                    if k == real_key:
                        return v
        return None

    @staticmethod
    def dict_replace_value_re(dict, real_key_list):
        for real_key in real_key_list:
            for (k, v) in dict.items():
                if k.startswith('@'):
                    k = k[1:]
                    match = re.match('.*' + k, real_key)
                    if match:
                        return re.sub(k, v, real_key)
                else:
                    if k == real_key:
                        return v
        return None

    def from_native(self):
        dic = {
            "int": "lua_pushnumber(L, ret);",
            "float": "lua_pushnumber(L, ret);",
            "char*": "lua_pushstring(L, ret);",
            "bool": "lua_pushboolean(L, ret);",
            "vec3": "Lua_PushVec3(L, ret.x, ret.y, ret.z);",
            "vec2": "Lua_PushVec2(L, ret.x, ret.y);",
        }

        func = ""
        if dic.has_key(self.name):
            func = dic[self.name]
        elif self.name.find("void") >= 0:
            print "ignor ", self.name
        else:
            print "Lua_PushCObject ", self.name
            func = "Lua_PushCObject(L, \"%s\", %s);" % (self.namespaced_name.rstrip("*"), "ret")
        return func

    def to_native(self, num):
        dic = {
            "int": "lua_tonumber(L, %d);",
            "float": "lua_tonumber(L, %d);",
            "char*": "lua_tostring(L, %d);"
        }
        
        func = ""
        if dic.has_key(self.name):
            func = dic[self.name] % (num+2)
        else:
            func = "(%s)lua_touserdata(L, %d);" % (self.name, num+2)

        return  self.whole_name + " arg" + str(num) + " = " + func

    def to_string(self, generator):
        conversions = generator.config['conversions']
        if conversions.has_key('native_types'):
            native_types_dict = conversions['native_types']
            if NativeType.dict_has_key_re(native_types_dict, [self.namespaced_name]):
                return NativeType.dict_get_value_re(native_types_dict, [self.namespaced_name])

        name = self.namespaced_name

        to_native_dict = generator.config['conversions']['to_native']
        from_native_dict = generator.config['conversions']['from_native']
        use_typedef = False

        typedef_name = self.canonical_type.name if None != self.canonical_type else None

        if None != typedef_name:
            if NativeType.dict_has_key_re(to_native_dict, [typedef_name]) or NativeType.dict_has_key_re(from_native_dict, [typedef_name]):
                use_typedef = True

        if use_typedef and self.canonical_type:
            name = self.canonical_type.namespaced_name
        return "const " + name if (self.is_pointer and self.is_const) else name

    def get_whole_name(self, generator):
        conversions = generator.config['conversions']
        to_native_dict = conversions['to_native']
        from_native_dict = conversions['from_native']
        use_typedef = False
        name = self.whole_name
        typedef_name = self.canonical_type.name if None != self.canonical_type else None

        if None != typedef_name:
            if NativeType.dict_has_key_re(to_native_dict, [typedef_name]) or NativeType.dict_has_key_re(from_native_dict, [typedef_name]):
                use_typedef = True

        if use_typedef and self.canonical_type:
            name = self.canonical_type.whole_name

        to_replace = None
        if conversions.has_key('native_types'):
            native_types_dict = conversions['native_types']
            to_replace = NativeType.dict_replace_value_re(native_types_dict, [name])

        if to_replace:
            name = to_replace

        return name

    def __str__(self):
        return  self.canonical_type.whole_name if None != self.canonical_type else self.whole_name