#version 330 core

precision highp float;

in vec3 position;
in vec3 normal;
in vec3 color;

in float LightIntensity;

out vec4 gl_FragColor;

void main() 
{ 
	gl_FragColor = vec4(1.0,1.0,1.0,1.0) * LightIntensity;
	//gl_FragColor = vec4(color,1.0) * LightIntensity;
} 
