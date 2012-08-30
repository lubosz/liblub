#version 330 core

uniform mat4 MVPMatrix;

in vec3 in_Vertex;

out vec3 reflectDir;

void main() {
    reflectDir = -in_Vertex;
    gl_Position    = MVPMatrix * vec4(in_Vertex,1);
}

