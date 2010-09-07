#version 150 core

uniform sampler2D FBOTex;
uniform sampler2D otherTex;

in vec2 TexCoord;

out vec4 FragColor;

void main()
{
	//FragColor    = texture(otherTex, TexCoord);
	FragColor    = texture(FBOTex, TexCoord);
	//FragColor    = vec4(TexCoord.x,TexCoord.y,0,1);
}

