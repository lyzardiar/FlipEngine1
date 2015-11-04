#include "base/ccConfig.h"
#ifndef __flipengine_prefiex_h__
#define __flipengine_prefiex_h__

#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

int register_all_flipengine_prefiex(lua_State* tolua_S);
