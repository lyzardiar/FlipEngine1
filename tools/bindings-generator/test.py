#!/usr/bin/env python
""" Usage: call with <filename> <typename>
"""
import os
import sys
from nativeclass import *
from clang import cindex

ignoredclass = {
    "lfStr":1,
    "hashtable":2,
    "DrawVert":3,
    "vec2":4,
    "vec3":5,
    "vec4":6,
    "mat4":7,
    "quat":8,
    "Joint":9,
    "aabb3d":10,
    "idMath":11,
    "color4":12,
    "Edge":13,
    "RenderSystemLocal":14
}
generateclass = {}

allheader = [
"ResourceSystem.h",
"Texture.h",
"Sprite.h",
"renderer/RenderSystem.h"
]

outfile = "../../Engine/autolua"


templatepath = os.path.join(os.path.dirname(__file__), "targets/lua")

def _pretty_print(diagnostics):
    print("====\nErrors in parsing headers:")
    severities=['Ignored', 'Note', 'Warning', 'Error', 'Fatal']
    for idx, d in enumerate(diagnostics):
        print "%s. <severity = %s,\n    location = %r,\n    details = %r>" % (
            idx+1, severities[d.severity], d.location, d.spelling)
    print("====\n")

def _deep_iterate(cursor, depth = 0):
    # get the canonical type
    if cursor.kind == cindex.CursorKind.CLASS_DECL:
        if cursor == cursor.type.get_declaration() and len(cursor.get_children_array()) > 0:
            print "class", cursor.displayname
            if not ignoredclass.has_key(cursor.displayname):
                nclass = NativeClass(cursor)
                generateclass[cursor.displayname] = nclass

    for node in cursor.get_children():
        # print("%s %s - %s" % (">" * depth, node.displayname, node.kind))
        _deep_iterate(node, depth + 1)

def _parse_header(index, args, header):
    tu = index.parse(header, args)

    if len(tu.diagnostics) > 0:
        _pretty_print(tu.diagnostics)
        is_fatal = False
        for d in tu.diagnostics:
            if d.severity >= cindex.Diagnostic.Error:
                is_fatal = True
        if is_fatal :
            print("*** Found errors - can not continue")
            # raise Exception("Fatal error in parsing headers")

    _deep_iterate(tu.cursor)

def _write_code(impl_file, template, search):
    code = Template(file=os.path.join(templatepath, "templates", template),
        searchList=[search])
    impl_file.write(str(code))

def main():
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

    args = ["-x", "c++", "-nostdinc", clanginc]
    args.extend(androidinc)
    args.extend(extra_flags)
    print args

    cindex.Config.set_library_path(clangdir)
    index = cindex.Index.create()

    for header in allheader:
        headerfile = os.path.join(srcdir, header)
        _parse_header(index, args, headerfile)
      
    headfilepath = os.path.join(outfile + ".h")
    base = os.path.basename(headfilepath)
    search = {
        "macro_judgement": base,
        "prefix": base,
        "out_file":base,
        "headers": allheader,
        "sorted_classes":[],
        "classlist":generateclass
    }

    head_file = open(headfilepath, "w+")
    _write_code(head_file, "layout_head.h", search)
    head_file.close()

    implfilepath = os.path.join(outfile + ".cpp")
    impl_file = open(implfilepath, "w+")
    _write_code(impl_file, "layout_head.c", search)
    for (k, nclass) in generateclass.items():
        nclass.generate_code(impl_file)
    _write_code(impl_file, "layout_foot.c", search)
    impl_file.close()

main()