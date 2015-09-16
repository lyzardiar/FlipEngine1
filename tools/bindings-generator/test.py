#!/usr/bin/env python
""" Usage: call with <filename> <typename>
"""

import sys
import clang.cindex

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

index = clang.cindex.Index.create()
tu = index.parse("E:/MySourceCode/FlipEngine1/tools/bindings-generator/Person.h")

if len(tu.diagnostics) > 0:
    _pretty_print(tu.diagnostics)
    is_fatal = False
    for d in tu.diagnostics:
        if d.severity >= cindex.Diagnostic.Error:
            is_fatal = True
    if is_fatal:
        print("*** Found errors - can not continue")
        raise Exception("Fatal error in parsing headers")

print 'Translation unit:', tu.spelling
find_typerefs(tu.cursor, "Person")