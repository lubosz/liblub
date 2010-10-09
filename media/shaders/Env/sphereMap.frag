#version 330 core

uniform sampler2D colorMap;
uniform sampler2D envMap;

in vec2 uv;
in vec2 uv2;

out vec4 fragColor;

void main()
{

	vec4 color = texture( colorMap, uv);
	vec4 env = texture( envMap, uv2);
	//fragColor = color + env*0.4;
	fragColor = env;
}

