#version 150 core
in float LightIntensity;
in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D myTexture;
void main()
{
    vec3 lightColor = vec3(texture(myTexture, TexCoord));
    FragColor    = vec4(lightColor * LightIntensity, 1.0);
}

