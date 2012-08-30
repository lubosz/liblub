#version 150 core


precision highp float;

in vec3 in_Vertex;
in vec3 in_Color;
in vec3 in_Normal;

uniform mat4 MVPMatrix;

out vec3 geom_Color;

void main(void) {

    //gl_Position = vec4(in_Position, 1.0);
    //geom_Color = in_Color;
    geom_Color = in_Normal;
	vec3 movePosition = vec3(in_Vertex.x + 3*gl_InstanceID,in_Vertex.y,in_Vertex.z);
gl_Position = MVPMatrix * vec4(movePosition,1);
}
