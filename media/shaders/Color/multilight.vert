#version 330 core

in vec3 in_Vertex;
in vec3 in_Normal;
in vec3 in_Tangent;
in vec2 in_Uv;

out vec4 positionView;
out vec3 normalView;
out vec3 normalWorld;

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;

out vec3 tangentView;
out vec2 uv;

void main() { 
	uv = in_Uv;
    vec4 position = vec4(in_Vertex,1);

	normalView = normalize(NormalMatrix * in_Normal);
	normalWorld = in_Normal;
	positionView = MVMatrix * position;

	gl_Position = MVPMatrix * position;
	
	//normalmapping
	tangentView = normalize(NormalMatrix * in_Tangent); 
}
