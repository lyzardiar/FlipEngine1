#!/usr/bin/env python
""" Usage: call with <filename> <typename>
"""
import os
import sys
from clang import cindex

def _pretty_print(diagnostics):
    print("====\nErrors in parsing headers:")
    severities=['Ignored', 'Note', 'Warning', 'Error', 'Fatal']
    for idx, d in enumerate(diagnostics):
        print "%s. <severity = %s,\n    location = %r,\n    details = %r>" % (
            idx+1, severities[d.severity], d.location, d.spelling)
    print("====\n")
        

def find_typerefs(node, typename):
    """ Find all references to the type named 'typename'
    """
    text = node.spelling or node.displayname
    print node.kind
    print text
    if node.kind.is_reference():
        ref_node = clang.cindex.Cursor_ref(node)
        print ref_node.spelling
        if ref_node.spelling == typename:
            print 'Found %s [line=%s, col=%s]' % (
                typename, node.location.line, node.location.column)
    # Recurse for children of this node
    for c in node.get_children():
        find_typerefs(c, typename)

def main():
    clang_dir = os.path.join(os.path.dirname(__file__), "libclang")
    print "clang_dir:", clang_dir

    project_dir = os.path.join(os.path.dirname(__file__), "../..")
    src_dir = os.path.join(project_dir, "engine")
    file_dir = os.path.join(src_dir, "Model.h")
    print(file_dir)

    ndk_root = "F:/AndroidTool/android-ndk-r9d"
    clangllvmdir = "F:/AndroidTool/android-ndk-r9d/toolchains/llvm-3.3/prebuilt/windows-x86_64"
    # project_inc = "-I%s/Engine/include" % project_dir

    clang_inc = "-I%s/lib/clang/3.3/include" % clangllvmdir
    android_inc = ["-I%s/platforms/android-14/arch-arm/usr/include" % ndk_root, 
    "-I%s/sources/cxx-stl/gnu-libstdc++/4.7/libs/armeabi-v7a/include" % ndk_root,
    "-I%s/sources/cxx-stl/gnu-libstdc++/4.7/include" % ndk_root]
    extra_flags = ["-D__WCHAR_MAX__=0x7fffffff", "-U__MINGW32__"]

    print clang_inc

    args = ["-x", "c++", clang_inc]
    args.extend(android_inc)
    args.extend(extra_flags)
    print args

    cindex.Config.set_library_path(clang_dir)
    index = cindex.Index.create()
    tu = index.parse(file_dir, args)

    if len(tu.diagnostics) > 0:
        _pretty_print(tu.diagnostics)
        is_fatal = False
        for d in tu.diagnostics:
            if d.severity >= cindex.Diagnostic.Error:
                is_fatal = True
        if is_fatal :
            print("*** Found errors - can not continue")
            raise Exception("Fatal error in parsing headers")

main()