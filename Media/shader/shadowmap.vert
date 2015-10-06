attribute vec4 vPosition;
uniform mat4 WVP;

void main() 
{
  gl_Position = WVP*vPosition;
}