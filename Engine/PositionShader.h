static const char gVertexShader[] =
"attribute vec4 vPosition;\n"
"attribute vec2 vTexCoord;\n"
"uniform mat4 MVP;\n"
"varying vec2 v_texCoord;\n"
"void main() {\n"
"  vec4 pos = MVP*vPosition;\n"
"  gl_Position = vec4(pos.x, pos.y, -1, 1);\n"
		"  v_texCoord = vTexCoord;\n"
    "}\n";

	static const char gFragmentShader[] =
		"precision mediump float;\n"
		"uniform sampler2D texture1;"
		"varying vec2 v_texCoord;\n"
		"void main() {\n"
		"   gl_FragColor = texture2D(texture1, v_texCoord);\n"
		//"   gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
		"}\n";
	

