#version 150 core

uniform sampler2D myTexture;

//in float LightIntensity;
in vec2 TexCoord;

out vec4 FragColor;

void main()
{
    vec3 lightColor = vec3(texture(myTexture, TexCoord));
    //FragColor    = vec4(lightColor * LightIntensity, 1.0);
	FragColor    = vec4(lightColor, 1.0);
}

