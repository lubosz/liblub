#version 410 core

uniform vec3 lightPosition;
uniform float g;
float g2 = g * g;

in vec3 direction;
in vec3 color1;
in vec3 color2;

out vec4 FragColor;

void main (void)
{
	float cosinus = dot(lightPosition, direction) / length(direction);
	float miePhase = 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + cosinus*cosinus) / pow(1.0 + g2 - 2.0*g*cosinus, 1.5);
    FragColor = vec4(color1,1) + miePhase * vec4(color2,1);
	FragColor.a = FragColor.b;
	//FragColor = vec4(color1,1);
}
