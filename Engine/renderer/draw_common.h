#ifndef __DRAW_COMMON_H__
#define __DRAW_COMMON_H__
#include "../r_public.h"

typedef void (*DrawFunc)(srfTriangles_t* geo);

void R_RenderPTPass(drawSurf_t* drawSurf, DrawFunc drawFunc);

void R_DrawPositonTex( srfTriangles_t* tri );

#endif


