uniform vec3 fvLightPosition;
uniform vec3 fvEyePosition;
uniform mat4 WVP;
uniform mat4 modelView;
uniform mat4 invModelView;

attribute vec3 vPosition;
attribute vec2 vTexCoord;
attribute vec3 vNormal;
attribute vec3 vTangent;
attribute vec3 vBinormal;

varying vec2 TexCoord;
varying vec3 ViewDirection;
varying vec3 LightDirection;
   
void main()
{
   gl_Position = WVP*vec4(vPosition, 1.0);
   TexCoord = vTexCoord;
    
   vec4 fvObjectPosition = modelView * vec4(vPosition, 1.0);
   vec3 pos = vec3(fvObjectPosition) / fvObjectPosition.w;

   vec3 fvViewDirection  = pos.xyz - fvEyePosition;
   vec3 fvLightDirection = pos.xyz - fvLightPosition;
   
   vec3 fvNormal         = vec3(invModelView * vec4(vNormal, 0.0));
   vec3 fvBinormal       = vec3(invModelView * vec4(vBinormal, 0.0));
   vec3 fvTangent        = vec3(invModelView * vec4(vTangent, 0.0));
      
   ViewDirection.x  = dot( fvTangent, fvViewDirection );
   ViewDirection.y  = dot( fvBinormal, fvViewDirection );
   ViewDirection.z  = dot( fvNormal, fvViewDirection );
   
   LightDirection.x  = dot( fvTangent, fvLightDirection );
   LightDirection.y  = dot( fvBinormal, fvLightDirection );
   LightDirection.z  = dot( fvNormal, fvLightDirection );
}