#include "vec2.h"

vec2::~vec2()
{
}

vec2& vec2::operator=(const vec2& other) { 
	x = other.x; y = other.y; 
	return *this; 
}

vec2 vec2::operator+(const vec2& other) {
	return vec2(x + other.x, y + other.y);	
}

vec2& vec2::operator+=(const vec2& other){ 
	x+=other.x; y+=other.y; 
	return *this; 
}