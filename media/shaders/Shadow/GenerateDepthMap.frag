#version 330 core

in vec4 position;

out float depthValue;

void main() 
{	
	depthValue = position.z;
}
