#version 330 core

//precision highp float;

in vec3 in_Vertex;
in vec3 in_Normal;
in vec3 in_Tangent;
in vec3 in_Bitangent;
in vec2 in_Uv;

out vec4 vpos;
out vec3 binormal;
out vec3 normal;
out vec3 tangent;
out vec2 texcoord;

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;

void main(){

	vpos = MVMatrix * vec4(in_Vertex,1);
	normal = NormalMatrix * in_Normal;
	tangent = NormalMatrix * in_Tangent; 
	binormal = NormalMatrix * in_Bitangent; 
	//binormal = cross(normal, tangent);
	
	texcoord = in_Uv;

	gl_Position = MVPMatrix * vec4(in_Vertex,1);
}
