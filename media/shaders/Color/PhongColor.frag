#version 330 core

precision highp float;

in vec4 positionCamView;
in vec3 normalCamView;

out vec4 fragColor;

uniform lightPositionCamView;

const float ambientFactor;

void main() 
{ 
	vec4 color = vec4(1.0,1.0,1.0,1.0);
	vec4 lightToFragment = positionCamView-lightPositionCamView;
	float lightIntensity = normalCamView.dot(vec3(lightToFragment.x,lightToFragment.y,lightToFragment.z));
	
	vec4 ambient = color * ambientFactor;
	vec4 diffuse = color * lightIntensity;
	vec4 specular = specular * lightIntensity;
	
	fragColor =  color + specular + ambient;
} 
