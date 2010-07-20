#version 330

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;
uniform vec3 LightPosition;
uniform float Time;

in vec3 in_Vertex;
in vec3 in_Normal;
in vec2 in_Uv;

out vec3 ObjPos;
out float V;
out float LightIntensity;

void main()
{
	ObjPos = (in_Vertex + vec3(0.0, 0.0, Time)) * 0.2;
	vec3 pos = vec3(MVMatrix * vec4(in_Vertex,1.0));
	vec3 tnorm = normalize(NormalMatrix * in_Normal);
	vec3 lightVec = normalize(LightPosition - pos);
	LightIntensity = max(dot(lightVec, tnorm), 0.0);
	V = in_Uv.x; // try .y for vertical stripes
	gl_Position = MVPMatrix * vec4(in_Vertex,1.0);
}
