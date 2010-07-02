#version 330 core

precision highp float;

in vec3 in_Vertex;
//in vec3 in_Color;
in vec3 in_Normal;
in vec3 in_Uv;

out vec3 geom_Color;

uniform mat4 MVPMatrix;

void main(void) {
    geom_Color = in_Normal;
    //gl_Position = MVPMatrix * vec4(in_Vertex,1);
	gl_Position = vec4(in_Vertex,1);
}
