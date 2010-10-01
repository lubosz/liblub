#version 330 core

in vec3 in_Vertex;
in vec3 in_Normal;
in vec3 in_Tangent;
in vec2 in_Uv;

out vec3 lightVec;
out vec3 eyeVec;
out vec2 uv;

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;

uniform vec4 lightPositionView;
					 

void main(void)
{

	vec4 positionView = MVMatrix * vec4(in_Vertex,1);
	gl_Position = MVPMatrix * vec4(in_Vertex,1);
	uv = in_Uv;
	vec3 n = normalize(NormalMatrix * in_Normal); 
	vec3 t = normalize(NormalMatrix * in_Tangent); 
	vec3 b = cross(n, t);

	vec3 tmpVec = lightPositionView.xyz - positionView.xyz;

	lightVec = vec3(
		dot(tmpVec, t),
		dot(tmpVec, b),
		dot(tmpVec, n)
	);

	tmpVec = -positionView.xyz;
	eyeVec.x = dot(tmpVec, t);
	eyeVec.y = dot(tmpVec, b);
	eyeVec.z = dot(tmpVec, n);
}

