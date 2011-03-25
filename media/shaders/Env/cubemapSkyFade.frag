#version 330 core

uniform samplerCube planetMap;
uniform samplerCube spaceMap;
uniform vec3 cameraPosition;

in vec3 reflectDir;

out vec4 fragColor;

void main() {
	float blend = max(0, min(length(cameraPosition) / 100.0, 1));
	fragColor = mix(texture(planetMap, reflectDir), texture(spaceMap, reflectDir), blend);
}

