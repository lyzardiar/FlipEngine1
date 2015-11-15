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
from nativefunction import *
from nativetype import *
from nativenamespace import *
from nativefield import *

class NativeClass(object):
    def __init__(self, cursor):
        # the cursor to the implementation
        self.target = os.path.join(os.path.dirname(__file__), "targets/lua")
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
                
    def generate_code(self, impl_file):
        for m in self.methods_clean():
            m['impl'].generate_code(impl_file)
        for m in self.static_methods_clean():
            m['impl'].generate_code(impl_file)
        for m in self.override_methods_clean():
            m['impl'].generate_code(impl_file)

        templatePath = os.path.join(os.path.dirname(__file__), "targets/lua")
        register = Template(file=os.path.join(templatePath, "templates", "register.c"),
                    searchList=[self])
        impl_file.write(str(register))

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

    def constructorState(self, cursor):
        m = NativeFunction(cursor)
        m.namespaced_class_name = self.namespaced_class_name
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

    def functionState(self, cursor):
        m = NativeFunction(cursor)
        m.namespaced_class_name = self.namespaced_class_name
        print "function " + cursor.displayname
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
                self.functionState(cursor)
            return True

        elif self._current_visibility == cindex.AccessSpecifierKind.PUBLIC and cursor.kind == cindex.CursorKind.CONSTRUCTOR:
            # Skip copy constructor
            if cursor.displayname == self.class_name + "(const " + self.namespaced_class_name + " &)":
                # print "Skip copy constructor: " + cursor.displayname
                return True
            self.constructorState(cursor)
            return True
        # else:
            # print >> sys.stderr, "unknown cursor: %s - %s" % (cursor.kind, cursor.displayname)
        return False

    def writemethods(self):
        code = ""
        for m in self.methods_clean():
            code += "       Lua_PushFunction(L, \"%s\", %s%s);\n"%(m['impl'].func_name, self.class_name, m['impl'].signature_name)
        return code