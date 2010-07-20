#version 330

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;
uniform vec3 LightPosition;

uniform float SpecularContribution;
uniform float DiffuseContribution;
uniform float Shininess;

in vec4 in_Vertex;
in vec3 in_Normal;
in vec2 in_Uv;

out float LightIntensity;
out vec3 Position;

void main()
{
	vec3 ecPosition = vec3(MVMatrix * in_Vertex);
	vec3 tnorm = normalize(NormalMatrix * in_Normal);
	vec3 lightVec = normalize(LightPosition - ecPosition);
	vec3 reflectVec = reflect(-lightVec, tnorm);
	vec3 viewVec = normalize(-ecPosition);
	float spec = max(dot(reflectVec, viewVec), 0.0);
	spec = pow(spec, Shininess);
	LightIntensity = DiffuseContribution *
	max(dot(lightVec, tnorm), 0.0) +
	SpecularContribution * spec;
	Position = vec3(in_Uv,1.0) * 5.0 - 2.5;
	gl_Position = MVPMatrix * in_Vertex;
}
