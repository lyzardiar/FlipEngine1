#include "autolua.h"
#include "ResourceSystem.h"
#include "Texture.h"
#include "Sprite.h"
#include "renderer/RenderSystem.h"


static int ResourceSystemAddText(lua_State* L)
{
    ResourceSystem* cobj = (ResourceSystem*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'AddText'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) 
    {
        const char* arg0 = lua_tostring(L, 2);
            Texture* ret = cobj->AddText(arg0);
            Lua_PushCObject(L, "Texture", ret);
    }

    return 0;
}

static int ResourceSystemAddMesh(lua_State* L)
{
    ResourceSystem* cobj = (ResourceSystem*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'AddMesh'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) 
    {
        const char* arg0 = lua_tostring(L, 2);
            StaticModel* ret = cobj->AddMesh(arg0);
            Lua_PushCObject(L, "StaticModel", ret);
    }

    return 0;
}

static int ResourceSystemFindShader(lua_State* L)
{
    ResourceSystem* cobj = (ResourceSystem*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'FindShader'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) 
    {
        int arg0 = lua_tonumber(L, 2);
            Shader* ret = cobj->FindShader(arg0);
            Lua_PushCObject(L, "Shader", ret);
    }

    return 0;
}

static int ResourceSystemLoadAllShader(lua_State* L)
{
    ResourceSystem* cobj = (ResourceSystem*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'LoadAllShader'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            bool ret = cobj->LoadAllShader();
            lua_pushboolean(L, ret);
    }

    return 0;
}

static int ResourceSystemAddTexture(lua_State* L)
{
    ResourceSystem* cobj = (ResourceSystem*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'AddTexture'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) 
    {
        const char* arg0 = lua_tostring(L, 2);
            Texture* ret = cobj->AddTexture(arg0);
            Lua_PushCObject(L, "Texture", ret);
    }

    return 0;
}

static int ResourceSystemAddShaderFromFile(lua_State* L)
{
    ResourceSystem* cobj = (ResourceSystem*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'AddShaderFromFile'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 2) 
    {
        const char* arg0 = lua_tostring(L, 2);
        const char* arg1 = lua_tostring(L, 3);
            Shader* ret = cobj->AddShaderFromFile(arg0, arg1);
            Lua_PushCObject(L, "Shader", ret);
    }

    return 0;
}

static int ResourceSystemAddMaterial(lua_State* L)
{
    ResourceSystem* cobj = (ResourceSystem*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'AddMaterial'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) 
    {
        const char* arg0 = lua_tostring(L, 2);
            Material* ret = cobj->AddMaterial(arg0);
            Lua_PushCObject(L, "Material", ret);
    }

    return 0;
}


int lua_register_ResourceSystem(lua_State* L)
{
    if (luaL_newmetatable(L, "ResourceSystem")) {
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
       Lua_PushFunction(L, "AddText", ResourceSystemAddText);
       Lua_PushFunction(L, "AddMesh", ResourceSystemAddMesh);
       Lua_PushFunction(L, "FindShader", ResourceSystemFindShader);
       Lua_PushFunction(L, "LoadAllShader", ResourceSystemLoadAllShader);
       Lua_PushFunction(L, "AddTexture", ResourceSystemAddTexture);
       Lua_PushFunction(L, "AddShaderFromFile", ResourceSystemAddShaderFromFile);
       Lua_PushFunction(L, "AddMaterial", ResourceSystemAddMaterial);

    }
    return 1;
}
static int MeshGetPositions(lua_State* L)
{
    Mesh* cobj = (Mesh*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'GetPositions'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            const void* ret = cobj->GetPositions();
            
    }

    return 0;
}

static int MeshGetPositionCount(lua_State* L)
{
    Mesh* cobj = (Mesh*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'GetPositionCount'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            int ret = cobj->GetPositionCount();
            lua_pushnumber(L, ret);
    }

    return 0;
}

static int MeshSetTexture(lua_State* L)
{
    Mesh* cobj = (Mesh*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'SetTexture'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) 
    {
        Texture* arg0 = (Texture*)lua_touserdata(L, 2);
            cobj->SetTexture(arg0);
    }

    return 0;
}

static int MeshGetIndices(lua_State* L)
{
    Mesh* cobj = (Mesh*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'GetIndices'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            const void* ret = cobj->GetIndices();
            
    }

    return 0;
}

static int MeshGetTexCoordCount(lua_State* L)
{
    Mesh* cobj = (Mesh*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'GetTexCoordCount'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            int ret = cobj->GetTexCoordCount();
            lua_pushnumber(L, ret);
    }

    return 0;
}

static int MeshGetNormalCount(lua_State* L)
{
    Mesh* cobj = (Mesh*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'GetNormalCount'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            int ret = cobj->GetNormalCount();
            lua_pushnumber(L, ret);
    }

    return 0;
}

static int MeshGetIndexCount(lua_State* L)
{
    Mesh* cobj = (Mesh*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'GetIndexCount'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            int ret = cobj->GetIndexCount();
            lua_pushnumber(L, ret);
    }

    return 0;
}

static int MeshHasNormals(lua_State* L)
{
    Mesh* cobj = (Mesh*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'HasNormals'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            bool ret = cobj->HasNormals();
            lua_pushboolean(L, ret);
    }

    return 0;
}

static int MeshGetColorCount(lua_State* L)
{
    Mesh* cobj = (Mesh*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'GetColorCount'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            int ret = cobj->GetColorCount();
            lua_pushnumber(L, ret);
    }

    return 0;
}

static int MeshGetTexture(lua_State* L)
{
    Mesh* cobj = (Mesh*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'GetTexture'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            Texture* ret = cobj->GetTexture();
            Lua_PushCObject(L, "Texture", ret);
    }

    return 0;
}

static int MeshGetNormals(lua_State* L)
{
    Mesh* cobj = (Mesh*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'GetNormals'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            const void* ret = cobj->GetNormals();
            
    }

    return 0;
}

static int MeshGetColors(lua_State* L)
{
    Mesh* cobj = (Mesh*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'GetColors'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            const void* ret = cobj->GetColors();
            
    }

    return 0;
}

static int MeshGetTexCoords(lua_State* L)
{
    Mesh* cobj = (Mesh*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'GetTexCoords'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            const void* ret = cobj->GetTexCoords();
            
    }

    return 0;
}

static int MeshSetPosition(lua_State* L)
{
    Mesh* cobj = (Mesh*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'SetPosition'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 3) 
    {
        float arg0 = lua_tonumber(L, 2);
        float arg1 = lua_tonumber(L, 3);
        float arg2 = lua_tonumber(L, 4);
            cobj->SetPosition(arg0, arg1, arg2);
    }

    return 0;
}

static int MeshGetTangentCount(lua_State* L)
{
    Mesh* cobj = (Mesh*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'GetTangentCount'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            int ret = cobj->GetTangentCount();
            lua_pushnumber(L, ret);
    }

    return 0;
}

static int MeshsetupVBO(lua_State* L)
{
    Mesh* cobj = (Mesh*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'setupVBO'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            cobj->setupVBO();
    }

    return 0;
}

static int MeshCalculateEdge(lua_State* L)
{
    Mesh* cobj = (Mesh*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'CalculateEdge'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            cobj->CalculateEdge();
    }

    return 0;
}

static int MeshHasTexCoords(lua_State* L)
{
    Mesh* cobj = (Mesh*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'HasTexCoords'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            bool ret = cobj->HasTexCoords();
            lua_pushboolean(L, ret);
    }

    return 0;
}


int lua_register_Mesh(lua_State* L)
{
    if (luaL_newmetatable(L, "Mesh")) {
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
       Lua_PushFunction(L, "GetPositions", MeshGetPositions);
       Lua_PushFunction(L, "GetPositionCount", MeshGetPositionCount);
       Lua_PushFunction(L, "SetTexture", MeshSetTexture);
       Lua_PushFunction(L, "GetIndices", MeshGetIndices);
       Lua_PushFunction(L, "GetTexCoordCount", MeshGetTexCoordCount);
       Lua_PushFunction(L, "GetNormalCount", MeshGetNormalCount);
       Lua_PushFunction(L, "GetIndexCount", MeshGetIndexCount);
       Lua_PushFunction(L, "HasNormals", MeshHasNormals);
       Lua_PushFunction(L, "GetColorCount", MeshGetColorCount);
       Lua_PushFunction(L, "GetTexture", MeshGetTexture);
       Lua_PushFunction(L, "GetNormals", MeshGetNormals);
       Lua_PushFunction(L, "GetColors", MeshGetColors);
       Lua_PushFunction(L, "GetTexCoords", MeshGetTexCoords);
       Lua_PushFunction(L, "SetPosition", MeshSetPosition);
       Lua_PushFunction(L, "GetTangentCount", MeshGetTangentCount);
       Lua_PushFunction(L, "setupVBO", MeshsetupVBO);
       Lua_PushFunction(L, "CalculateEdge", MeshCalculateEdge);
       Lua_PushFunction(L, "HasTexCoords", MeshHasTexCoords);

    }
    return 1;
}
static int SpriteSetLabel(lua_State* L)
{
    Sprite* cobj = (Sprite*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'SetLabel'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) 
    {
        const char* arg0 = lua_tostring(L, 2);
            cobj->SetLabel(arg0);
    }

    return 0;
}

static int SpriteSetTexture(lua_State* L)
{
    Sprite* cobj = (Sprite*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'SetTexture'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) 
    {
        const char* arg0 = lua_tostring(L, 2);
            cobj->SetTexture(arg0);
    }

    return 0;
}

static int SpriteLookAtView(lua_State* L)
{
    Sprite* cobj = (Sprite*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'LookAtView'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) 
    {
        mat4* arg0 = (mat4*)lua_touserdata(L, 2);
            cobj->LookAtView(arg0);
    }

    return 0;
}



static int SpriteSetupVBO(lua_State* L)
{
    Sprite* cobj = (Sprite*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'SetupVBO'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            cobj->SetupVBO();
    }

    return 0;
}

static int SpriteSetPosition(lua_State* L)
{
    Sprite* cobj = (Sprite*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'SetPosition'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 3) 
    {
        float arg0 = lua_tonumber(L, 2);
        float arg1 = lua_tonumber(L, 3);
        float arg2 = lua_tonumber(L, 4);
            cobj->SetPosition(arg0, arg1, arg2);
    }

    return 0;
}



int lua_register_Sprite(lua_State* L)
{
    if (luaL_newmetatable(L, "Sprite")) {
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
       Lua_PushFunction(L, "SetLabel", SpriteSetLabel);
       Lua_PushFunction(L, "SetTexture", SpriteSetTexture);
       Lua_PushFunction(L, "LookAtView", SpriteLookAtView);
       Lua_PushFunction(L, "SetupVBO", SpriteSetupVBO);
       Lua_PushFunction(L, "SetPosition", SpriteSetPosition);

    }
    return 1;
}
static int TextureGetName(lua_State* L)
{
    Texture* cobj = (Texture*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'GetName'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            int ret = cobj->GetName();
            lua_pushnumber(L, ret);
    }

    return 0;
}


int lua_register_Texture(lua_State* L)
{
    if (luaL_newmetatable(L, "Texture")) {
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
       Lua_PushFunction(L, "GetName", TextureGetName);

    }
    return 1;
}
static int RenderSystemAddDrawSur(lua_State* L)
{
    RenderSystem* cobj = (RenderSystem*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'AddDrawSur'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) 
    {
        drawSurf_t* arg0 = (drawSurf_t*)lua_touserdata(L, 2);
            bool ret = cobj->AddDrawSur(arg0);
            lua_pushboolean(L, ret);
    }

    return 0;
}

static int RenderSystemGetNumSurf(lua_State* L)
{
    RenderSystem* cobj = (RenderSystem*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'GetNumSurf'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            int ret = cobj->GetNumSurf();
            lua_pushnumber(L, ret);
    }

    return 0;
}

static int RenderSystemAddStaticModel(lua_State* L)
{
    RenderSystem* cobj = (RenderSystem*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'AddStaticModel'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) 
    {
        StaticModel* arg0 = (StaticModel*)lua_touserdata(L, 2);
            bool ret = cobj->AddStaticModel(arg0);
            lua_pushboolean(L, ret);
    }

    return 0;
}

static int RenderSystemAddSprite(lua_State* L)
{
    RenderSystem* cobj = (RenderSystem*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'AddSprite'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) 
    {
        Sprite* arg0 = (Sprite*)lua_touserdata(L, 2);
            bool ret = cobj->AddSprite(arg0);
            lua_pushboolean(L, ret);
    }

    return 0;
}

static int RenderSystemAddUISurf(lua_State* L)
{
    RenderSystem* cobj = (RenderSystem*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'AddUISurf'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) 
    {
        drawSurf_t* arg0 = (drawSurf_t*)lua_touserdata(L, 2);
            bool ret = cobj->AddUISurf(arg0);
            lua_pushboolean(L, ret);
    }

    return 0;
}

static int RenderSystemInit(lua_State* L)
{
    RenderSystem* cobj = (RenderSystem*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'Init'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            cobj->Init();
    }

    return 0;
}

static int RenderSystemDrawString(lua_State* L)
{
    RenderSystem* cobj = (RenderSystem*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'DrawString'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 1) 
    {
        const char* arg0 = lua_tostring(L, 2);
            cobj->DrawString(arg0);
    }

    return 0;
}

static int RenderSystemFrameUpdate(lua_State* L)
{
    RenderSystem* cobj = (RenderSystem*)lua_touserdata(L,1);
    if (!cobj) 
    {
        luaL_error(L,"invalid 'cobj' in function 'FrameUpdate'", nullptr);
        return 0;
    }

    int argc = lua_gettop(L)-1;
    if (argc == 0) 
    {
            cobj->FrameUpdate();
    }

    return 0;
}


int lua_register_RenderSystem(lua_State* L)
{
    if (luaL_newmetatable(L, "RenderSystem")) {
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
       Lua_PushFunction(L, "AddDrawSur", RenderSystemAddDrawSur);
       Lua_PushFunction(L, "GetNumSurf", RenderSystemGetNumSurf);
       Lua_PushFunction(L, "AddStaticModel", RenderSystemAddStaticModel);
       Lua_PushFunction(L, "AddSprite", RenderSystemAddSprite);
       Lua_PushFunction(L, "AddUISurf", RenderSystemAddUISurf);
       Lua_PushFunction(L, "Init", RenderSystemInit);
       Lua_PushFunction(L, "DrawString", RenderSystemDrawString);
       Lua_PushFunction(L, "FrameUpdate", RenderSystemFrameUpdate);

    }
    return 1;
}
int register_all_auto(lua_State* L)
{
	lua_register_ResourceSystem(L);
	lua_register_Mesh(L);
	lua_register_Sprite(L);
	lua_register_Texture(L);
	lua_register_RenderSystem(L);
	return 0;
}
