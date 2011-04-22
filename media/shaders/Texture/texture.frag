#version 330 core

uniform sampler2D diffuse;

in vec2 uv;

out vec4 fragColor;

uniform float scaleUv;

void main()
{
	fragColor    = texture(diffuse, uv*scaleUv);
//fragColor.a = 1.0;
}

