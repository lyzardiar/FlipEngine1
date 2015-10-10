const vec4 fvAmbient = vec4(1.0, 1.0, 1.0, 1.0);
const vec4 fvSpecular = vec4(1.0, 1.0, 1.0, 1.0);
const vec4 fvDiffuse = vec4(1.0, 1.0, 1.0, 1.0);
const float fSpecularPower = 25.0;

uniform sampler2D texture1;
uniform sampler2D bumpMap;

varying vec2 TexCoord;
varying vec3 ViewDirection;
varying vec3 LightDirection;

void main( void )
{
   vec3  fvLightDirection = normalize( LightDirection );
   vec3  fvNormal         = normalize( ( texture2D( bumpMap, Texcoord ).xyz * 2.0 ) - 1.0 );
   float fNDotL           = dot( fvNormal, -fvLightDirection ); 
   
   vec3  fvReflection     = normalize( ( ( 2.0 * fvNormal ) * fNDotL ) + fvLightDirection ); 
   vec3  fvViewDirection  = normalize( ViewDirection );
   float fRDotV           = max( 0.0, dot( fvReflection, -fvViewDirection ) );
   
   vec4  fvBaseColor      = texture2D( texture1, TexCoord );

   vec4  fvTotalAmbient   = fvAmbient * fvBaseColor; 
   vec4  fvTotalDiffuse   = fvDiffuse * fNDotL * fvBaseColor; 
   vec4  fvTotalSpecular  = fvSpecular * ( pow( fRDotV, fSpecularPower ) );
  
   gl_FragColor = ( fvTotalAmbient + fvTotalDiffuse + fvTotalSpecular );
   //gl_FragColor = vec4(fvNormal, 0);
}