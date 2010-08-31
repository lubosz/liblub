#version 330 core
precision highp float;

in vec3 in_Vertex;
in vec3 in_Normal;
in vec3 in_Color;

out vec3 position;
out vec3 normal;
out vec3 color;

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;

uniform vec3 LightPosition;

const float SpecularContribution = 0.3;
const float DiffuseContribution = 1.0 - SpecularContribution;

out float LightIntensity;

void main() 
{ 
    vec4 MCvertex = vec4(in_Vertex,1);
    //vec3 ecPosition =  vec3(MVMatrix * MCvertex);
    vec3 ecPosition =  vec3(MVMatrix * MCvertex);
    vec3 tnorm      =  normalize(NormalMatrix * in_Normal);
    vec3 lightVec =  normalize(NormalMatrix * LightPosition);
    //vec3 lightVec   =  normalize(LightPosition - ecPosition);
	//vec3 lightVec   =  normalize(LightPosition);
    vec3 reflectVec =  reflect(-lightVec, tnorm);
    vec3 viewVec    =  normalize(-ecPosition);
    float diffuse   =  max(dot(lightVec, tnorm), 0.0);
    float spec      =  0.0;
    if (diffuse > 0.0)
    {
        spec = max(dot(reflectVec, viewVec), 0.0);
        spec = pow(spec, 16.0);
    }
    LightIntensity  = DiffuseContribution * diffuse +
                       SpecularContribution * spec;

	position = in_Vertex; 
	normal = in_Normal; 
	color = in_Color;
	
	gl_Position = MVPMatrix * MCvertex;

}
