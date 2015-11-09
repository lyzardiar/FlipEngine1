#!/usr/bin/env python
""" Usage: call with <filename> <typename>
"""
import os
import sys
from generatorcode import *
from clang import cindex

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
            # is_targeted_class = True
            # if self.cpp_ns:
            #     is_targeted_class = False
            #     namespaced_name = get_namespaced_name(cursor)
            #     for ns in self.cpp_ns:
            #         if namespaced_name.startswith(ns):
            #             is_targeted_class = True
            #             break

            nclass = NativeClass(cursor)
            nclass.generate_code()

    for node in cursor.get_children():
        # print("%s %s - %s" % (">" * depth, node.displayname, node.kind))
        _deep_iterate(node, depth + 1)

def generate_code(filePath, tempPath, outFile):
    stream = file(os.path.join(tempPath, "conversions.yaml"), "r")
    implfilepath = os.path.join(outFile + ".cpp")
    headfilepath = os.path.join(outFile + ".hpp")

    base = os.path.basename(headfilepath)
    search = {
        "macro_judgement": base,
        "prefix": base,
        "out_file":base,
        "headers":[],
        "sorted_classes":[]
    }

    impl_file = open(implfilepath, "w+")
    head_file = open(headfilepath, "w+")

    layout_h = Template(file=os.path.join(tempPath, "templates", "layout_head.h"),
                        searchList=[search])

    layout_c = Template(file=os.path.join(tempPath, "templates", "layout_head.c"),
                        searchList=[search])

    apidoc_ns_script = Template(file=os.path.join(tempPath, "templates", "apidoc_ns.script"),
                            searchList=[search])
    head_file.write(str(layout_h))
    impl_file.write(str(layout_c))

    # _parse_headers()

    layout_h = Template(file=os.path.join(tempPath, "templates", "layout_foot.h"),
                        searchList=[search])

    layout_c = Template(file=os.path.join(tempPath, "templates", "layout_foot.c"),
                        searchList=[search])
    head_file.write(str(layout_h))
    impl_file.write(str(layout_c))

    impl_file.close()
    head_file.close()

def main():
    clang_dir = os.path.join(os.path.dirname(__file__), "libclang")
    print "clang_dir:", clang_dir

    project_dir = os.path.join(os.path.dirname(__file__), "../..")
    src_dir = os.path.join(project_dir, "engine")
    file_dir = os.path.join(src_dir, "common/vec2.h")
    print(file_dir)

    ndk_root = "C:/program1/android-ndk-r9d"
    clangllvmdir = os.path.join(ndk_root, "toolchains/llvm-3.3/prebuilt/windows")
    # project_inc = "-I%s/Engine/include" % project_dir

    clang_inc = "-I%s/lib/clang/3.3/include" % clangllvmdir
    android_inc = ["-I%s/platforms/android-14/arch-arm/usr/include" % ndk_root, 
    "-I%s/sources/cxx-stl/gnu-libstdc++/4.7/libs/armeabi-v7a/include" % ndk_root,
    "-I%s/sources/cxx-stl/gnu-libstdc++/4.7/include" % ndk_root]
    extra_flags = ["-D__WCHAR_MAX__=0x7fffffff", "-U__MINGW32__"]

    tempPath = os.path.join(os.path.dirname(__file__), "targets/lua")
    outfile = os.path.join(os.path.dirname(__file__), "out/vec2")

    args = ["-x", "c++", "-nostdinc", clang_inc]
    args.extend(android_inc)
    args.extend(extra_flags)
    print args

    cindex.Config.set_library_path(clang_dir)
    index = cindex.Index.create()

    generate_code(src_dir, tempPath, outfile)
    # tu = index.parse(file_dir, args)

    # if len(tu.diagnostics) > 0:
    #     _pretty_print(tu.diagnostics)
    #     is_fatal = False
    #     for d in tu.diagnostics:
    #         if d.severity >= cindex.Diagnostic.Error:
    #             is_fatal = True
    #     if is_fatal :
    #         print("*** Found errors - can not continue")
    #         raise Exception("Fatal error in parsing headers")

    # _deep_iterate(tu.cursor)


main()