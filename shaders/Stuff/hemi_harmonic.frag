#version 420 core

in vec3  DiffuseColor;
out vec4 fragColor;

void main() {
  fragColor = vec4(DiffuseColor,1);
}

