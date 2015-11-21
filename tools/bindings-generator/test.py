#!/usr/bin/env python
""" Usage: call with <filename> <typename>
"""
import os
import sys
from nativeclass import *
from clang import cindex


func_name = "AddSprite"

f = funcname_tolower(func_name)
print f