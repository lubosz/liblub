#version 330 core

uniform sampler2D myTexture;

in vec2 uv;

out vec4 fragColor;

void main()
{
	fragColor    = texture(myTexture, uv);
//fragColor.a = 1.0;
}

