#!/usr/bin/env python
import os
import sys
from nativeclass import *
from clang import cindex

templatepath = os.path.join(os.path.dirname(__file__), "targets/lua")

class codegenerator:
    def __init__(self):
        clangdir = os.path.join(os.path.dirname(__file__), "libclang")
        projectdir = os.path.join(os.path.dirname(__file__), "../..")
        srcdir = os.path.join(projectdir, "engine")
        ndk_root = "C:/program1/android-ndk-r9d"
        clangllvmdir = os.path.join(ndk_root, "toolchains/llvm-3.3/prebuilt/windows")
        # project_inc = "-I%s/Engine/include" % project_dir
        clanginc = "-I%s/lib/clang/3.3/include" % clangllvmdir
        androidinc = ["-I%s/platforms/android-14/arch-arm/usr/include" % ndk_root, 
        "-I%s/sources/cxx-stl/gnu-libstdc++/4.7/libs/armeabi-v7a/include" % ndk_root,
        "-I%s/sources/cxx-stl/gnu-libstdc++/4.7/include" % ndk_root]
        extra_flags = ["-D__WCHAR_MAX__=0x7fffffff", "-U__MINGW32__"]
        # outFile = os.path.join(os.path.dirname(__file__), "out/autolua")
        self.args = ["-x", "c++", "-nostdinc", clanginc]
        self.args.extend(androidinc)
        self.args.extend(extra_flags)

        cindex.Config.set_library_path(clangdir)
        self.index = cindex.Index.create()
        self.generateclass = {}
        self.tmpclass = []

    def _pretty_print(self, diagnostics):
        print("====\nErrors in parsing headers:")
        severities=['Ignored', 'Note', 'Warning', 'Error', 'Fatal']
        for idx, d in enumerate(diagnostics):
            print "%s. <severity = %s,\n    location = %r,\n    details = %r>" % (
            idx+1, severities[d.severity], d.location, d.spelling)
            print("====\n")

    def _deep_iterate(self, cursor, depth = 0):
        # get the canonical type
        if cursor.kind == cindex.CursorKind.CLASS_DECL:
            if cursor == cursor.type.get_declaration() and len(cursor.get_children_array()) > 0:
                if not self.generateclass.has_key(cursor.displayname):
                    nclass = NativeClass(cursor)
                    self.generateclass[cursor.displayname] = nclass
                self.tmpclass.append(self.generateclass[cursor.displayname])

        for node in cursor.get_children():
            # print("%s %s - %s" % (">" * depth, node.displayname, node.kind))
            self._deep_iterate(node, depth + 1)

    def _parse_header(self, header):
        if not os.path.isfile(header):
            print "error _parse_header", header
            return

        self.tmpclass = []
        tu = self.index.parse(header, self.args)

        if len(tu.diagnostics) > 0:
            self._pretty_print(tu.diagnostics)
            is_fatal = False
            for d in tu.diagnostics:
                if d.severity >= cindex.Diagnostic.Error:
                    is_fatal = True
            if is_fatal :
                print("*** Found errors - can not continue")
                # raise Exception("Fatal error in parsing headers")

        self._deep_iterate(tu.cursor)



