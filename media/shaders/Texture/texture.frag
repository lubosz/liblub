#version 150 core

uniform sampler2D myTexture;

in vec2 TexCoord;

out vec4 FragColor;

void main()
{
	FragColor    = texture(myTexture, TexCoord);
}

