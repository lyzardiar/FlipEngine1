#!/usr/bin/env python
# generator.py
# simple C++ generator, originally targetted for Spidermonkey bindings
#
# Copyright (c) 2011 - Zynga Inc.

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

INVALID_NATIVE_TYPE = "??"

default_arg_type_arr = [

# An integer literal.
cindex.CursorKind.INTEGER_LITERAL,

# A floating point number literal.
cindex.CursorKind.FLOATING_LITERAL,

# An imaginary number literal.
cindex.CursorKind.IMAGINARY_LITERAL,

# A string literal.
cindex.CursorKind.STRING_LITERAL,

# A character literal.
cindex.CursorKind.CHARACTER_LITERAL,

# [C++ 2.13.5] C++ Boolean Literal.
cindex.CursorKind.CXX_BOOL_LITERAL_EXPR,

# [C++0x 2.14.7] C++ Pointer Literal.
cindex.CursorKind.CXX_NULL_PTR_LITERAL_EXPR,

# An expression that refers to some value declaration, such as a function,
# varible, or enumerator.
cindex.CursorKind.DECL_REF_EXPR
]

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


def build_namespace(cursor, namespaces=[]):
    '''
    build the full namespace for a specific cursor
    '''
    if cursor:
        parent = cursor.semantic_parent
        if parent:
            if parent.kind == cindex.CursorKind.NAMESPACE or parent.kind == cindex.CursorKind.CLASS_DECL:
                namespaces.append(parent.displayname)
                build_namespace(parent, namespaces)

    return namespaces


def get_namespaced_name(declaration_cursor):
    ns_list = build_namespace(declaration_cursor, [])
    ns_list.reverse()
    ns = "::".join(ns_list)
    if len(ns) > 0:
        return ns + "::" + declaration_cursor.displayname
    return declaration_cursor.displayname

def generate_namespace_list(cursor, namespaces=[]):
    '''
    build the full namespace for a specific cursor
    '''
    if cursor:
        parent = cursor.semantic_parent
        if parent:
            if parent.kind == cindex.CursorKind.NAMESPACE or parent.kind == cindex.CursorKind.CLASS_DECL:
                if parent.kind == cindex.CursorKind.NAMESPACE:
                    namespaces.append(parent.displayname)
                generate_namespace_list(parent, namespaces)
    return namespaces

def get_namespace_name(declaration_cursor):
    ns_list = generate_namespace_list(declaration_cursor, [])
    ns_list.reverse()
    ns = "::".join(ns_list)

    if len(ns) > 0:
        return ns + "::"

    return declaration_cursor.displayname


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

    def from_native(self, convert_opts):
        assert(convert_opts.has_key('generator'))
        generator = convert_opts['generator']
        keys = []

        if self.canonical_type != None:
            keys.append(self.canonical_type.name)
        keys.append(self.name)

        from_native_dict = generator.config['conversions']['from_native']

        if self.is_object:
            if not NativeType.dict_has_key_re(from_native_dict, keys):
                keys.append("object")
        elif self.is_enum:
            keys.append("int")

        if NativeType.dict_has_key_re(from_native_dict, keys):
            tpl = NativeType.dict_get_value_re(from_native_dict, keys)
            tpl = Template(tpl, searchList=[convert_opts])
            return str(tpl).rstrip()

        return "#pragma warning NO CONVERSION FROM NATIVE FOR " + self.name

    def to_native(self, convert_opts):
        assert('generator' in convert_opts)
        generator = convert_opts['generator']
        keys = []

        if self.canonical_type != None:
            keys.append(self.canonical_type.name)
        keys.append(self.name)

        to_native_dict = generator.config['conversions']['to_native']
        if self.is_object:
            if not NativeType.dict_has_key_re(to_native_dict, keys):
                keys.append("object")
        elif self.is_enum:
            keys.append("int")

        if self.is_function:
            tpl = Template(file=os.path.join(generator.target, "templates", "lambda.c"),
                searchList=[convert_opts, self])
            indent = convert_opts['level'] * "\t"
            return str(tpl).replace("\n", "\n" + indent)


        if NativeType.dict_has_key_re(to_native_dict, keys):
            tpl = NativeType.dict_get_value_re(to_native_dict, keys)
            tpl = Template(tpl, searchList=[convert_opts])
            return str(tpl).rstrip()
        return "#pragma warning NO CONVERSION TO NATIVE FOR " + self.name + "\n" + convert_opts['level'] * "\t" +  "ok = false"

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

class NativeField(object):
    def __init__(self, cursor):
        cursor = cursor.canonical
        self.cursor = cursor
        self.name = cursor.displayname
        self.kind = cursor.type.kind
        self.location = cursor.location
        member_field_re = re.compile('m_(\w+)')
        match = member_field_re.match(self.name)
        if match:
            self.pretty_name = match.group(1)
        else:
            self.pretty_name = self.name

# return True if found default argument.
def iterate_param_node(param_node, depth=1):
    for node in param_node.get_children():
        # print(">"*depth+" "+str(node.kind))
        if node.kind in default_arg_type_arr:
            return True

        if iterate_param_node(node, depth + 1):
            return True

    return False

class NativeFunction(object):
    def __init__(self, cursor):
        self.cursor = cursor
        self.func_name = cursor.spelling
        self.signature_name = self.func_name
        self.arguments = []
        self.argumtntTips = []
        self.static = cursor.kind == cindex.CursorKind.CXX_METHOD and cursor.is_static_method()
        self.implementations = []
        self.is_constructor = False
        self.not_supported = False
        self.is_override = False
        self.ret_type = NativeType.from_type(cursor.result_type)
        self.comment = self.get_comment(cursor.getRawComment())

        # parse the arguments
        # if self.func_name == "spriteWithFile":
        #   pdb.set_trace()
        for arg in cursor.get_arguments():
            self.argumtntTips.append(arg.spelling)

        for arg in cursor.type.argument_types():
            nt = NativeType.from_type(arg)
            self.arguments.append(nt)
            # mark the function as not supported if at least one argument is not supported
            if nt.not_supported:
                self.not_supported = True

        found_default_arg = False
        index = -1

        for arg_node in self.cursor.get_children():
            if arg_node.kind == cindex.CursorKind.CXX_OVERRIDE_ATTR:
                self.is_override = True
            if arg_node.kind == cindex.CursorKind.PARM_DECL:
                index += 1
                if iterate_param_node(arg_node):
                    found_default_arg = True
                    break

        self.min_args = index if found_default_arg else len(self.arguments)

    def get_comment(self, comment):
        replaceStr = comment

        if comment is None:
            return ""

        regular_replace_list = [
            ("(\s)*//!",""),
            ("(\s)*//",""),
            ("(\s)*/\*\*",""),
            ("(\s)*/\*",""),
            ("\*/",""),
            ("\r\n", "\n"),
            ("\n(\s)*\*", "\n"),
            ("\n(\s)*@","\n"),
            ("\n(\s)*","\n"), 
            ("\n(\s)*\n", "\n"),
            ("^(\s)*\n",""), 
            ("\n(\s)*$", ""),
            ("\n","<br>\n"),
            ("\n", "\n-- ")
        ]

        for item in regular_replace_list:
            replaceStr = re.sub(item[0], item[1], replaceStr)


        return replaceStr

 


class NativeOverloadedFunction(object):
    def __init__(self, func_array):
        self.implementations = func_array
        self.func_name = func_array[0].func_name
        self.signature_name = self.func_name
        self.min_args = 100
        self.is_constructor = False
        for m in func_array:
            self.min_args = min(self.min_args, m.min_args)

        self.comment = self.get_comment(func_array[0].cursor.getRawComment())

    def get_comment(self, comment):
        replaceStr = comment

        if comment is None:
            return ""

        regular_replace_list = [
            ("(\s)*//!",""),
            ("(\s)*//",""),
            ("(\s)*/\*\*",""),
            ("(\s)*/\*",""),
            ("\*/",""),
            ("\r\n", "\n"),
            ("\n(\s)*\*", "\n"),
            ("\n(\s)*@","\n"),
            ("\n(\s)*","\n"), 
            ("\n(\s)*\n", "\n"),
            ("^(\s)*\n",""), 
            ("\n(\s)*$", ""),
            ("\n","<br>\n"),
            ("\n", "\n-- ")
        ]

        for item in regular_replace_list:
            replaceStr = re.sub(item[0], item[1], replaceStr)

        return replaceStr

    def append(self, func):
        self.min_args = min(self.min_args, func.min_args)
        self.implementations.append(func)

    def generate_code(self, current_class=None, is_override=False):
        gen = current_class.generator
        config = gen.config
        static = self.implementations[0].static
        tpl = Template(file=os.path.join(gen.target, "templates", "function.h"),
                        searchList=[current_class, self])
        if not is_override:
            gen.head_file.write(str(tpl))
        if static:
            if config['definitions'].has_key('sfunction'):
                tpl = Template(config['definitions']['sfunction'],
                                searchList=[current_class, self])
                self.signature_name = str(tpl)
            tpl = Template(file=os.path.join(gen.target, "templates", "sfunction_overloaded.c"),
                            searchList=[current_class, self])
        else:
            if not self.is_constructor:
                if config['definitions'].has_key('ifunction'):
                    tpl = Template(config['definitions']['ifunction'],
                                    searchList=[current_class, self])
                    self.signature_name = str(tpl)
            else:
                if config['definitions'].has_key('constructor'):
                    tpl = Template(config['definitions']['constructor'],
                                    searchList=[current_class, self])
                    self.signature_name = str(tpl)
            tpl = Template(file=os.path.join(gen.target, "templates", "ifunction_overloaded.c"),
                            searchList=[current_class, self])
        if not is_override:
            print str(tpl)
            gen.impl_file.write(str(tpl))

        if current_class != None:
            apidoc_function_overload_script = Template(file=os.path.join(gen.target,
                                                    "templates",
                                                    "apidoc_function_overload.script"),
                                  searchList=[current_class, self])
            current_class.doc_func_file.write(str(apidoc_function_overload_script))
      

class NativeClass(object):
    def __init__(self, cursor):
        # the cursor to the implementation
        self.target = os.path.join(os.path.dirname(__file__), "targets/lua")
        print self.target
        self.cursor = cursor
        self.class_name = cursor.displayname
        self.parents = []
        self.fields = []
        self.methods = {}
        self.static_methods = {}
        self._current_visibility = cindex.AccessSpecifierKind.PRIVATE
        #for generate lua api doc
        self.override_methods = {}
        self.has_constructor  = False

        self.namespaced_class_name = get_namespaced_name(cursor)
        self.namespace_name        = get_namespace_name(cursor)
        self.parse()

    @property
    def underlined_class_name(self):
        return self.namespaced_class_name.replace("::", "_")

    def parse(self):
        print "parse class", self.class_name
        self._deep_iterate(self.cursor)

    def methods_clean(self):
        '''
        clean list of methods (without the ones that should be skipped)
        '''
        ret = []
        for name, impl in self.methods.iteritems():
            should_skip = False
            if name == 'constructor':
                should_skip = True

            if not should_skip:
                ret.append({"name": name, "impl": impl})
        return ret

    def static_methods_clean(self):
        '''
        clean list of static methods (without the ones that should be skipped)
        '''
        ret = []
        # for name, impl in self.static_methods.iteritems():
            # should_skip = self.generator.should_skip(self.class_name, name)
            # if not should_skip:
            #     ret.append({"name": name, "impl": impl})
        return ret

    def override_methods_clean(self):
        '''
        clean list of override methods (without the ones that should be skipped)
        '''
        ret = []
        # for name, impl in self.override_methods.iteritems():
        #     should_skip = self.generator.should_skip(self.class_name, name)
        #     if not should_skip:
        #         ret.append({"name": name, "impl": impl})
        return ret

   # def generate_code(self, current_class=None, generator=None, is_override=False):
   #  tpl = Template(file=os.path.join(gen.target, "templates", "function.h"),
   #                  searchList=[current_class, self])
   #  if not is_override:
   #      gen.head_file.write(str(tpl))
   #  if self.static:
   #      if config['definitions'].has_key('sfunction'):
   #          tpl = Template(config['definitions']['sfunction'],
   #                              searchList=[current_class, self])
   #          self.signature_name = str(tpl)
   #      tpl = Template(file=os.path.join(gen.target, "templates", "sfunction.c"),
   #                      searchList=[current_class, self])
   #  else:
   #      if not self.is_constructor:
   #          if config['definitions'].has_key('ifunction'):
   #              tpl = Template(config['definitions']['ifunction'],
   #                              searchList=[current_class, self])
   #              self.signature_name = str(tpl)
   #      else:
   #          if config['definitions'].has_key('constructor'):
   #              tpl = Template(config['definitions']['constructor'],
   #                              searchList=[current_class, self])
   #              self.signature_name = str(tpl)
   #      if self.is_constructor and gen.script_type == "spidermonkey" :
   #          tpl = Template(file=os.path.join(gen.target, "templates", "constructor.c"),
   #                                          searchList=[current_class, self])
   #      else :
   #          tpl = Template(file=os.path.join(gen.target, "templates", "ifunction.c"),
   #                          searchList=[current_class, self])
   #  if not is_override:
   #      gen.impl_file.write(str(tpl))
   #  apidoc_function_script = Template(file=os.path.join(gen.target,
   #                                                  "templates",
   #                                                  "apidoc_function.script"),
   #                                searchList=[current_class, self])
   #  if gen.script_type == "spidermonkey":
   #      gen.doc_file.write(str(apidoc_function_script))
   #  else:
   #      if gen.script_type == "lua" and current_class != None :
   #          current_class.doc_func_file.write(str(apidoc_function_script))
                
    def generate_code(self):
        '''
        actually generate the code. it uses the current target templates/rules in order to
        generate the right code
        '''
        # config = self.generator.config


        # if self.generator.script_type == "lua":
        #     docfuncfilepath = os.path.join(self.generator.outdir + "/api", self.class_name + ".lua")
        #     self.doc_func_file = open(docfuncfilepath, "w+")
        #     apidoc_fun_head_script  = Template(file=os.path.join(self.generator.target,
        #                                                  "templates",
        #                                                  "apidoc_function_head.script"),
        #                                searchList=[{"current_class": self}])
        #     self.doc_func_file.write(str(apidoc_fun_head_script))

        # self.generator.head_file.write(str(prelude_h))
        # self.generator.impl_file.write(str(prelude_c))
        # self.generator.doc_file.write(str(apidoc_classhead_script))
        for m in self.methods_clean():
            m['impl'].generate_code(self)
        for m in self.static_methods_clean():
            m['impl'].generate_code(self)
        for m in self.override_methods_clean():
            m['impl'].generate_code(self, is_override = True)
        # # generate register section
        # register = Template(file=os.path.join(self.generator.target, "templates", "register.c"),
        #                     searchList=[{"current_class": self}])
        # apidoc_classfoot_script = Template(file=os.path.join(self.generator.target,
        #                                                  "templates",
        #                                                  "apidoc_classfoot.script"),
        #                                searchList=[{"current_class": self}])
        # self.generator.impl_file.write(str(register))
        # self.generator.doc_file.write(str(apidoc_classfoot_script))
        # if self.generator.script_type == "lua":
        #     apidoc_fun_foot_script  = Template(file=os.path.join(self.generator.target,
        #                                                  "templates",
        #                                                  "apidoc_function_foot.script"),
        #                                searchList=[{"current_class": self}])
        #     self.doc_func_file.write(str(apidoc_fun_foot_script))
        #     self.doc_func_file.close()
    def _deep_iterate(self, cursor=None, depth=0):
        for node in cursor.get_children():
            # print("%s%s - %s" % ("> " * depth, node.displayname, node.kind))
            if self._process_node(node):
                self._deep_iterate(node, depth + 1)

    @staticmethod
    def _is_method_in_parents(current_class, method_name):
        if len(current_class.parents) > 0:
            if method_name in current_class.parents[0].methods:
                return True
            return NativeClass._is_method_in_parents(current_class.parents[0], method_name)
        return False

    def _is_ref_class(self, depth = 0):
        """
        Mark the class as 'cocos2d::Ref' or its subclass.
        """
        # print ">" * (depth + 1) + " " + self.class_name

        if len(self.parents) > 0:
            return self.parents[0]._is_ref_class(depth + 1)

        if self.is_ref_class:
            return True

        return False

    def _process_node(self, cursor):
        '''
        process the node, depending on the type. If returns true, then it will perform a deep
        iteration on its children. Otherwise it will continue with its siblings (if any)

        @param: cursor the cursor to analyze
        '''
        if cursor.kind == cindex.CursorKind.CXX_BASE_SPECIFIER:
            parent = cursor.get_definition()
            parent_name = parent.displayname
            print cursor.displayname
            print parent_name


        elif cursor.kind == cindex.CursorKind.FIELD_DECL:
            self.fields.append(NativeField(cursor))
        elif cursor.kind == cindex.CursorKind.CXX_ACCESS_SPEC_DECL:
            self._current_visibility = cursor.get_access_specifier()
        elif cursor.kind == cindex.CursorKind.CXX_METHOD and cursor.get_availability() != cindex.AvailabilityKind.DEPRECATED:
            # skip if variadic
            if self._current_visibility == cindex.AccessSpecifierKind.PUBLIC and not cursor.type.is_function_variadic():
                print "NativeFunction " + cursor.displayname
                m = NativeFunction(cursor)
                # bail if the function is not supported (at least one arg not supported)
                registration_name = m.func_name
                if m.not_supported:
                    return False
                if m.is_override:
                    if NativeClass._is_method_in_parents(self, registration_name):
                        if not self.override_methods.has_key(m.func_name):
                            self.override_methods[registration_name] = m
                        else:
                            previous_m = self.override_methods[registration_name]
                            if isinstance(previous_m, NativeOverloadedFunction):
                                previous_m.append(m)
                            else:
                                self.override_methods[registration_name] = NativeOverloadedFunction([m, previous_m])
                        return False

                if m.static:
                    if not self.static_methods.has_key(registration_name):
                        self.static_methods[registration_name] = m
                    else:
                        previous_m = self.static_methods[registration_name]
                        if isinstance(previous_m, NativeOverloadedFunction):
                            previous_m.append(m)
                        else:
                            self.static_methods[registration_name] = NativeOverloadedFunction([m, previous_m])
                else:
                    if not self.methods.has_key(registration_name):
                        self.methods[registration_name] = m
                    else:
                        previous_m = self.methods[registration_name]
                        if isinstance(previous_m, NativeOverloadedFunction):
                            previous_m.append(m)
                        else:
                            self.methods[registration_name] = NativeOverloadedFunction([m, previous_m])
            return True

        elif self._current_visibility == cindex.AccessSpecifierKind.PUBLIC and cursor.kind == cindex.CursorKind.CONSTRUCTOR:
            # Skip copy constructor
            if cursor.displayname == self.class_name + "(const " + self.namespaced_class_name + " &)":
                # print "Skip copy constructor: " + cursor.displayname
                return True

            m = NativeFunction(cursor)
            m.is_constructor = True
            self.has_constructor = True
            if not self.methods.has_key('constructor'):
                self.methods['constructor'] = m
            else:
                previous_m = self.methods['constructor']
                if isinstance(previous_m, NativeOverloadedFunction):
                    previous_m.append(m)
                else:
                    m = NativeOverloadedFunction([m, previous_m])
                    m.is_constructor = True
                    self.methods['constructor'] = m
            return True
        # else:
            # print >> sys.stderr, "unknown cursor: %s - %s" % (cursor.kind, cursor.displayname)
        return False