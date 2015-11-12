#pragma once

#ifdef __cplusplus
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#endif

int RegisterAllEngine(lua_State* tolua_S);