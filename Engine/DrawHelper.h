#pragma once
#include "glUtils.h"
#include "Mesh.h"
#include "FPSCounter.h"
#define  RENDER_WIDTH  800.0f
#define  RENDER_HEIGHT 600
void draw3DCoordinate();
void drawBone(vec3 orgin, vec3 dir);
void drawBox();
/* A sphere with proper normals and texture coords */
void drawSphereMesh(float radius, unsigned int polyCountX, unsigned int polyCountY);

inline void drawTexture(GLuint tex)
{
	 glEnable(GL_TEXTURE_2D);
	 glUseProgram(0);
	 glMatrixMode(GL_PROJECTION);
	 glLoadIdentity();
	 glOrtho(-RENDER_WIDTH/2,RENDER_WIDTH/2,-RENDER_HEIGHT/2,RENDER_HEIGHT/2,1,20);
	 glMatrixMode(GL_MODELVIEW);
	 glLoadIdentity();
	 glColor4f(1,1,1,1);
	 glActiveTexture(GL_TEXTURE0);

	 glBindTexture(GL_TEXTURE_2D, tex);

	 glTranslated(0,0,-1);
	 glBegin(GL_QUADS);
	 glTexCoord2d(0,0);glVertex3f(0,0,0);
	 glTexCoord2d(1,0);glVertex3f(RENDER_WIDTH/2,0,0);
	 glTexCoord2d(1,1);glVertex3f(RENDER_WIDTH/2,RENDER_HEIGHT/2,0);
	 glTexCoord2d(0,1);glVertex3f(0,RENDER_HEIGHT/2,0);
	 
	 glEnd();
	 glDisable(GL_TEXTURE_2D);
}


