#version 330 core

precision highp float;

in vec3 in_Vertex;
in vec3 in_Normal;
in vec3 in_Tangent;
in vec2 in_Uv;

out vec4 positionView;
out vec3 normalView;
//out vec3 lightVec;
//out vec3 eyeVec;
out vec2 uv;

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;

uniform vec4 lightPositionView;

void main() 
{ 
#ifdef useDiffuseTexture
	uv = in_Uv;
#endif
	positionView = MVMatrix * vec4(in_Vertex,1);
	normalView = normalize(NormalMatrix * in_Normal); 

	vec4 positionProjection = MVPMatrix * vec4(in_Vertex,1);
/*
	//Normal Mapping
	vec3 tangentView = NormalMatrix * in_Tangent; 
	vec3 b = cross(normalView, tangentView);

	vec3 tmpVec = lightPositionView.xyz - positionView.xyz;

	lightVec = vec3(
		dot(tmpVec, tangentView),
		dot(tmpVec, b),
		dot(tmpVec, normalView)
	);

	tmpVec = -positionView.xyz;
	eyeVec.x = dot(tmpVec, tangentView);
	eyeVec.y = dot(tmpVec, b);
	eyeVec.z = dot(tmpVec, normalView);
	*/
	gl_Position = positionProjection;
}
