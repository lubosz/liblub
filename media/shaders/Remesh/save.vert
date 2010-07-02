#version 120

void main(void)
{
	gl_Position = gl_Vertex;
	gl_FrontColor=gl_Color;
	gl_TexCoord[0]=vec4(gl_Normal,0.0);
}
