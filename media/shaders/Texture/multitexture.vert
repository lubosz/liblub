#version 150 core

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;
uniform vec3 LightPosition;

in vec3 in_Vertex;
in vec3 in_Normal;
in vec2 in_Uv;

out vec2 TexCoord;
out float LightIntensity;
//out vec4 TexCoord;

const float specularContribution = 0.1;
const float diffuseContribution = 1.0 - specularContribution;

void main()
{
    vec3 ecPosition =  vec3(MVMatrix * vec4(in_Vertex,1));
    vec3 tnorm      =  normalize(NormalMatrix * in_Normal);
    vec3 lightVec   =  normalize(LightPosition - ecPosition);
    vec3 reflectVec =  reflect(-lightVec, tnorm);
    vec3 viewVec    =  normalize(-ecPosition);
    float spec      = clamp(dot(reflectVec, viewVec), 0.0, 1.0);
    spec            = pow(spec, 16.0);
    LightIntensity  = diffuseContribution
                       * max(dot(lightVec, tnorm), 0.0)
                       + specularContribution * spec;
    //TexCoord        = TexCoord0.st;
    TexCoord        = in_Uv;
    gl_Position     = MVPMatrix * vec4(in_Vertex,1);
}

