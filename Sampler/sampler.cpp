/************************************************************************/
/* blur shader & outline shader
/************************************************************************/

#include "sampler.h"
#include "Mesh.h"
#include "ResourceSystem.h"
#include "renderer/RenderSystem.h"
#include "../media/KnightModel.h"
#include "r_public.h"
#include "sys/sys_public.h"
#include "Camera.h"
#include "Sprite.h"
#include "Mesh.h"
#include "MeshLoader3DS.h"
#include "Shader.h"

#include "Shape.h"

#include "Model_lwo.h"
#include "Model.h"
#include "ScriptSystem.h"

#include "luautils.h"


#pragma comment(lib, "FlipEngine.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

Game* game = new ShadowSampler();
Texture* _texture;
Box* box;
float angle = 0;

ShadowSampler::ShadowSampler( void ):_camera(NULL)
	,_defaultSprite(NULL)
{
}

ShadowSampler::~ShadowSampler()
{
}

void ShadowSampler::Init()
{
	_scriptSys = new ScriptSystem;
	_scriptSys->Init();
	
	lua_State* L = _scriptSys->GetLuaState();
	
	//SetupCamera();
	_camera = new Camera;
	_camera->Setup3DCamera(800, 600);
	_camera->Walk(-10);

	// rendersys
	RenderSystem** renderSys = (RenderSystem **)lua_newuserdata(L, sizeof(RenderSystem*));
	*renderSys = _renderSys;
	lua_setfield(L, LUA_GLOBALSINDEX, "renderSys");

	// camera
	Camera** camera = (Camera**)lua_newuserdata(L, sizeof(Camera*));
	*camera = _camera;
	Lua_SetMetatable(L, "Camera");
	lua_setfield(L, LUA_GLOBALSINDEX, "camera");

	//RenderSystem* r = (RenderSystem*)lua_touserdata(L, 1);

	//_scriptSys->RunScript("script/main.lua");
	
	box = _renderSys->CreateBox();
	//box->SetPosition(0, 0, -10);

	box->SetViewProj(_camera->GetViewProj());
	_renderSys->AddDrawSur(box->_drawSurf);
}

void ShadowSampler::Frame()
{
	sysEvent_t ev = Sys_GetEvent();
	while (ev.evType != SE_NONE )
	{
		ProcessEvent(&ev);
		ev = Sys_GetEvent();
	}

	//_scriptSys->Call("frameUpdate");

	//glCullFace(GL_BACK);
	//glUseProgram(0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//vec2 screenpos = _spLight->ToScreenCoord(*_camera->GetViewProj());

	//vec3 pos = _camera->GetPosition();
	//char buff[255];
	//sprintf_s(buff, "camera position %.02f %.02f %.02f screen pos: %.02f %.02f", pos.x, pos.y, 
	//	pos.z, screenpos.x, screenpos.y);
	//_defaultSprite->SetLabel(buff);
}

void ShadowSampler::ProcessEvent(sysEvent_s* event)
{
	switch (event->evType)
	{
	case SE_KEY:
		{
			if (event->evValue2 == 1)
			{
				quat q;
				q.fromAxisAngle(vec3(0, 1, 0), angle);
				angle += QUAT_PI / 4;
				box->SetRotation(q.x, q.y, q.z, q.w);
			}

			//_scriptSys->CallFuncI("onKey", event->evValue);
		}
		break;
	case SE_MOUSE:
		{
			HitTest(event->evValue, event->evValue2);
		}
	default:
		break;
	}
}

bool ShadowSampler::HitTest(int mouseX, int mouseY)
{
	return false;
	int width = 1366;
	int height = 768;
	float x = (2.0f * mouseX) / width - 1.0f;
	float y = 1.0f - (2.0f * mouseY) / height;
	float z = 1.0f;
	vec3 ray_nds = vec3 (x, y, z);

	vec4 ray_clip = vec4 (ray_nds, 1.0);

	mat4 invViewProj = _camera->GetViewProj()->inverse();
	vec4 ray_world = invViewProj * ray_clip;

	if (ray_world.w != 0.0)
	{
		ray_world.x /= ray_world.w;
		ray_world.y /= ray_world.w;
		ray_world.z /= ray_world.w;			
	}

	vec3 ray_dir = ray_world.ToVec3() - _camera->GetPosition();

	Sys_Printf("hit test");

	//drawSurf_t* drawSurf = testModel->getSurfaces()[0];
	//float scale;
	//if( drawSurf->geo->aabb.RayIntersection(_camera->GetPosition(), ray_dir, scale) )
	//	drawSurf->bHit = true;
	//else
	//	drawSurf->bHit = false;

	return true;
}

void ShadowSampler::SetupCamera()
{
	_camera = new Camera();
	_camera->Setup3DCamera(1366, 768);
	_camera->SetPosition(-1.26f, 1.26f, 1.8f);
	//_camera->SetPosition(-126.f, 126.f, 100.f);
}

void ShadowSampler::CreateLight()
{
	_light = new Camera();
	_light->Setup3DCamera(1366, 768);
	_light->SetPosition(-1.f, 1.f, -1.f);

	_spLight = new Sprite;
	_spLight->SetLabel("light");
	_spLight->SetPosition(-100.f, 100.f, -100.f);
	_renderSys->AddSprite(_spLight);
}

