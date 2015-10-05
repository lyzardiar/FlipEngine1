attribute vec4 vPosition;
attribute vec2 vTexCoord;
uniform mat4 WVP;
varying vec2 v_texCoord;

void main() 
{
  gl_Position = WVP*vPosition;
  v_texCoord = vTexCoord;
}