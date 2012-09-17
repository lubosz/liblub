#version {{ version }}

precision highp float;

in vec3 in_Vertex;
in vec3 in_Normal;
in vec3 in_Tangent;
in vec3 in_Biangent;
in vec2 in_Uv;

out vec4 positionWS;
out vec3 normalWS;
out vec3 tangentWS;
out vec3 binormalWS;
out vec2 uv;

uniform mat4 MMatrix;
uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;

void main(){
	positionWS = MMatrix * vec4(in_Vertex,1);
	normalWS = normalize((MMatrix * vec4(in_Normal,0)).xyz);
	tangentWS = normalize((MMatrix * vec4(in_Tangent,0)).xyz);
	//binormalWS = normalize((MMatrix * vec4(in_Biangent,0)).xyz);
	binormalWS = cross(normalWS, tangentWS);
	uv = in_Uv;
	gl_Position = MVPMatrix * vec4(in_Vertex,1);
}
