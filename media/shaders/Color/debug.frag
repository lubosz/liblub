#version 150 core

uniform sampler2D myTexture;

in vec3 normal;
in vec2 uv;
out vec4 FragColor;

void main()
{
	//FragColor = vec4(uv,0,1);
	FragColor    = texture(myTexture, uv);
}

