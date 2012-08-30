#version 330 core

uniform vec3 LightPosition;
uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;

in vec3 in_Vertex;
in vec3 in_Normal;

out float LightIntensity;
out vec3 ReflectDir;

void main()
{
    gl_Position    = MVPMatrix * vec4(in_Vertex,1);
    vec3 normal    = normalize(NormalMatrix * in_Normal);
    vec4 pos       = MVMatrix * vec4(in_Vertex,1);
    vec3 eyeDir    = pos.xyz;
    ReflectDir     = reflect(-eyeDir, normal);
    vec3 lightDir  = normalize(LightPosition - eyeDir);
    LightIntensity = max(dot(lightDir, normal),0.0);
}

