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
from nativetype import *

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
        self.export = False
        self.cursor = cursor
        self.displayname = cursor.displayname
        self.func_name = cursor.spelling
        self.signature_name = self.func_name
        self.arguments = []
        self.argumtntTips = []
        self.ret_type = NativeType.from_type(cursor.result_type)

        self.static = cursor.kind == cindex.CursorKind.CXX_METHOD and cursor.is_static_method()
        self.implementations = []
        self.is_constructor = False
        self.not_supported = False
        self.is_override = False
        self.comment = self.get_comment(cursor.getRawComment())
        self.namespaced_class_name = ""
        # parse the arguments
        # if self.func_name == "spriteWithFile":
        #   pdb.set_trace()
        for arg in cursor.get_arguments():
            self.argumtntTips.append(arg.spelling)

        # print self.argumtntTips

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

    def generate_code(self, impl_file):
        templatePath = os.path.join(os.path.dirname(__file__), "targets/lua")
        if self.static:
            tpl = Template(file=os.path.join(templatePath, "templates", "sfunction.c"),
                searchList=[self])
        else:
            tpl = Template(file=os.path.join(templatePath, "templates", "ifunction.c"),
                            searchList=[self])
        impl_file.write(str(tpl))


class NativeOverloadedFunction(object):
    def __init__(self, func_array):
        self.export = False
        self.implementations = func_array
        self.func_name = func_array[0].func_name
        self.signature_name = self.func_name
        self.min_args = 100
        self.is_constructor = False
        self.ret_type = func_array[0].ret_type
        self.displayname = func_array[0].displayname

        for m in func_array:
            self.min_args = min(self.min_args, m.min_args)

    def append(self, func):
        self.min_args = min(self.min_args, func.min_args)
        self.implementations.append(func)

    def generate_code(self, impl_file):
        print "overloaded function ", self.func_name
