#version 150 core

precision highp float;

in vec3 in_Vertex;
in vec3 in_Color;
in vec3 in_Normal;

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;

out vec3 geom_Color;

void main(void) {

   	geom_Color = in_Color;
	float pointSize = 50.0;
	//vec3 movePosition = vec3(in_Vertex.x + 3*gl_InstanceID,in_Vertex.y,in_Vertex.z);
	//gl_Position = MVPMatrix * vec4(movePosition,1);
	vec3 pos_eye = vec3(MVMatrix * vec4(in_Vertex,1.0));
	gl_PointSize = max(1.0, pointSize / (1.0 - pos_eye.z)); 


	gl_Position = MVPMatrix * vec4(in_Vertex,1);
}
