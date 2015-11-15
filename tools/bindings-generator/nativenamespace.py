from clang import cindex

INVALID_NATIVE_TYPE = "??"

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
