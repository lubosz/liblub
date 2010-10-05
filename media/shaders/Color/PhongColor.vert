#version 330 core

precision highp float;

in vec3 in_Vertex;
in vec3 in_Normal;
in vec3 in_Tangent;
in vec2 in_Uv;

out vec4 positionView;
out vec3 normalView;
#ifdef useNormalTexture
out vec3 lightVec;
out vec3 eyeVec;
#endif
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
	
#ifdef useNormalTexture
	vec3 n = normalize(NormalMatrix * in_Normal); 
	vec3 t = normalize(NormalMatrix * in_Tangent); 
	vec3 b = cross(n, t);

	vec3 lightDirection = lightPositionView.xyz - positionView.xyz;

	lightVec = vec3(
		dot(lightDirection, t),
		dot(lightDirection, b),
		dot(lightDirection, n)
	);

	eyeVec.x = dot(-positionView.xyz, t);
	eyeVec.y = dot(-positionView.xyz, b);
	eyeVec.z = dot(-positionView.xyz, n);
#endif
	gl_Position = positionProjection;
}
