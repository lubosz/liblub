#version 150 core

uniform sampler2D FBOTex;

in vec2 TexCoord;

out vec4 FragColor;

void main()
{
	FragColor    = texture(FBOTex, TexCoord);
	//FragColor    = vec4(TexCoord.x,TexCoord.y,0,1);
}

