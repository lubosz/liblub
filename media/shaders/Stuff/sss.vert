#version 330 core

precision highp float;

in vec3 in_Vertex;
in vec3 in_Normal;
in vec3 in_Tangent;
in vec2 in_Uv;

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;

uniform vec4 lightPositionView;

out vec3 worldNormal, eyeVec, lightVec, vertPos, lightPos;
out vec2 uv;

void subScatterVS(in vec4 ecVert)
{
	lightVec = lightPositionView.xyz - ecVert.xyz;
	eyeVec = -ecVert.xyz;
	vertPos = ecVert.xyz;
	lightPos = lightPositionView.xyz;
}

void main()
{
	worldNormal = NormalMatrix * in_Normal;

	vec4 ecPos = MVPMatrix * vec4(in_Vertex,1);

	subScatterVS(ecPos);

	gl_Position = ecPos;

	uv = in_Uv;
}
