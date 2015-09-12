#pragma once
#include "color4.h"
#include "vec3.h"

enum E_LIGHT_TYPE
{
	ELT_POINT,
	ELT_SPOT,
	ELT_DIRECTIONAL,
	ELT_COUNT
};

class Light
{
public:
	Light() : AmbientColor(0.f,0.f,0.f), DiffuseColor(1.f,1.f,1.f),
		SpecularColor(1.f,1.f,1.f), Attenuation(1.f,0.f,0.f),
		OuterCone(45.f), InnerCone(0.f), Falloff(2.f),
		Position(0.f,0.f,0.f), Direction(0.f,0.f,1.f),
		Radius(100.f), Type(ELT_POINT), CastShadows(true)
		{}


	//! Ambient color emitted by the light
	color4 AmbientColor;

	//! Diffuse color emitted by the light.
	/** This is the primary color you want to set. */
	color4 DiffuseColor;

	//! Specular color emitted by the light.
	/** For details how to use specular highlights, see SMaterial::Shininess */
	color4 SpecularColor;

	//! Attenuation factors (constant, linear, quadratic)
	/** Changes the light strength fading over distance.
	Can also be altered by setting the radius, Attenuation will change to
	(0,1.f/radius,0). Can be overridden after radius was set. */
	vec3 Attenuation;

	//! The angle of the spot's outer cone. Ignored for other lights.
	float OuterCone;

	//! The angle of the spot's inner cone. Ignored for other lights.
	float InnerCone;

	//! The light strength's decrease between Outer and Inner cone.
	float Falloff;

	//! Read-ONLY! Position of the light.
	/** If Type is ELT_DIRECTIONAL, it is ignored. Changed via light scene node's position. */
	vec3 Position;

	//! Read-ONLY! Direction of the light.
	/** If Type is ELT_POINT, it is ignored. Changed via light scene node's rotation. */
	vec3 Direction;

	//! Read-ONLY! Radius of light. Everything within this radius will be lighted.
	float Radius;

	//! Read-ONLY! Type of the light. Default: ELT_POINT
	E_LIGHT_TYPE Type;

	//! Read-ONLY! Does the light cast shadows?
	bool CastShadows:1;
};

