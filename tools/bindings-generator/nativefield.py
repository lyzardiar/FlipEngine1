from clang import cindex
import re

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
        # print cursor.displayname