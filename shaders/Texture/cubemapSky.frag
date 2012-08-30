#version 330 core

uniform samplerCube EnvMap;

in vec3 reflectDir;

out vec4 FragColor;

void main() {
	FragColor = texture(EnvMap, reflectDir);
}

