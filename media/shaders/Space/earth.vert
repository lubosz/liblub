#version 330 core

in vec3 in_Vertex;
in vec3 in_Normal;
in vec2 in_Uv;

out float Diffuse;
out vec3 Specular;
out vec2 uv;

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;

uniform vec4 lightPositionView;

void main()
{
    vec3 ecPosition =  vec3(MVMatrix * vec4(in_Vertex,1));
    vec3 tnorm      =  normalize(NormalMatrix * in_Normal);
    vec3 lightVec   =  normalize(lightPositionView.xyz - ecPosition);
    vec3 reflectVec =  reflect(-lightVec, tnorm);
    vec3 viewVec    =  normalize(-ecPosition);
    float spec      = clamp(dot(reflectVec, viewVec), 0.0, 1.0);
    spec            = pow(spec, 8.0);
    Specular        = spec * vec3(1.0, 0.941, 0.898) * 0.3;
    Diffuse         = max(dot(lightVec, tnorm), 0.0);
    uv        = in_Uv;
    gl_Position     = MVPMatrix * vec4(in_Vertex,1);
}

