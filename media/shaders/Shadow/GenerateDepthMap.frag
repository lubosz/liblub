#version 330 core

in vec4 position;

//out float depthValue;

out vec4 fragColor;

void main() 
{	
	float depthValue = position.z/20;
	fragColor = vec4(depthValue,depthValue,depthValue,depthValue);
}
