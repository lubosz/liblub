#version 330 core

in  vec3 in_Vertex;
in  vec3 in_Normal;
in  vec3 in_Tangent;
in  vec2 in_Uv;

out vec3 LightDir;
out vec3 EyeDir;
out vec2 uv;

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;

uniform vec4 lightPositionView;

void main()
{
	gl_Position = MVPMatrix * vec4(in_Vertex,1);
	EyeDir = vec3(MVMatrix * vec4(in_Vertex,1));
	uv = in_Uv;
	vec3 n = normalize(NormalMatrix * in_Normal);
	vec3 t = normalize(NormalMatrix * in_Tangent);
	vec3 b = cross(n, t);
	vec3 v;
	v.x = dot(lightPositionView.xyz, t);
	v.y = dot(lightPositionView.xyz, b);
	v.z = dot(lightPositionView.xyz, n);
	LightDir = normalize(v);
	v.x = dot(EyeDir, t);
	v.y = dot(EyeDir, b);
	v.z = dot(EyeDir, n);
	EyeDir = normalize(v);
}

