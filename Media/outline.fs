uniform sampler2D texture1;
varying vec2 v_texCoord;
float viewport_inv_width = 1.0/153.f;
float viewport_inv_height = 1.0/115.f;

const float4 samples[6] = 
{
	float4(-1.0, 1.0, 0., 1.0),
	float4(0.0,  1.0, 0., 2.0),
	float4(1.0,  1.0, 0., 1.0),
	float4(-1.0, -1.0, 0., 1.0),
	float4(0.0,  -1.0, 0., 2.0),
	float4(1.0,  -1.0, 0., 1.0)
};

void main() 
{
	float4 col = float4(0.0, 0.0, 0.0, 0.0);

	for(int i=0; i<4; i++)
		col += samples[i].w * texture2D(texture1, (v_texCoord) + float2(
		samples[i].x * viewport_inv_width, samples[i].y * viewport_inv_height));

	gl_FragColor = col;
};