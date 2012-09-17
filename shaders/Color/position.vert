#version {{ version }}

precision highp float;

in vec3 in_Vertex;

uniform mat4 MVPMatrix;

out vec3 geom_Color;

void main(void) {
	geom_Color = in_Vertex;
	gl_Position = MVPMatrix * vec4(in_Vertex,1);
}
