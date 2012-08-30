#version 330 core

precision highp float;

in vec3 in_Vertex;
in vec3 in_Color;

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;

out vec3 geom_Color;

const	float initialSize = 50.0;

void main(void) {
  geom_Color = in_Color;
	vec3 pos_eye = vec3(MVMatrix * vec4(in_Vertex,1.0));
	gl_PointSize = max(1.0, initialSize / (1.0 - pos_eye.z));
	gl_Position = MVPMatrix * vec4(in_Vertex,1);
}
