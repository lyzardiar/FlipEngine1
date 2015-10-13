#ifndef __DRAWVERT_H__
#define __DRAWVERT_H__

#include "common/vec2.h"
#include "common/vec3.h"

class DrawVert {
public:
	vec3			xyz;
	vec2			st;
	vec3			normal;
	vec3			tangents[2];
	unsigned char color[4];
 
	float		operator[]( const int index ) const;
	float &		operator[]( const int index );

	void			Clear( void )
	{
		xyz.Zero();
		st.Zero();
		normal.Zero();
		tangents[0].Zero();
		tangents[1].Zero();
		color[0] = color[1] = color[2] = color[3] = 0;
	}

	void			Lerp( const DrawVert &a, const DrawVert &b, const float f );
	void			LerpAll( const DrawVert &a, const DrawVert &b, const float f );

	void			Normalize( void );

	//void		SetColor(  color );
	//dword		GetColor( void ) const;
};




#endif



