#version 330 core

uniform sampler2D RenderTexture;

in vec2 uv;

out vec4 fragColor;

void main()
{
	vec4 color = texture2D(RenderTexture, uv);

	float intensity = 0.0;
	if(color.r==0.0 && color.g==0.0 && color.b==0.0) intensity=1.0;
	fragColor = vec4(vec3(intensity),1.0);
}

