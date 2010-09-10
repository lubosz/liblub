#version 330 core

uniform sampler2D RenderTexture;

in vec2 TexCoord;

out vec4 FragColor;

void main()
{
	vec4 color = texture2D(RenderTexture, TexCoord);

	float intensity = 0.0;
	if(color.r==1.0 && color.g==1.0 && color.b==1.0) intensity=1.0;
	FragColor = vec4(vec3(intensity),1.0);
}

