#include "RenderSystem.h"
#include "../glutils.h"

RenderSystemLocal::RenderSystemLocal(glimpParms_t *glimpParms_t)
{
	GL_CreateDevice(glimpParms_t);
}

void RenderSystemLocal::Init()
{
}

void RenderSystemLocal::FrameUpdate()
{
	//Sys_Printf("common_frame");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RB_SetGL2D();

	  glColor3f (1.0, 0.0, 0.0);
       glPointSize(5);
	 glBegin(GL_POINTS);
       for (int i = 0; i < 7; i++)
          glVertex2f (i*100, 100);
	 
	 glEnd();

	GL_SwapBuffers();
}