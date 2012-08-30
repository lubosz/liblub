#version 420 core

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;
//uniform vec3 LightDir; // Light direction in eye coordinates
//uniform vec4 ViewPosition;

const vec3 LightDir = vec3(1,0,0); // Light direction in eye coordinates
const vec4 ViewPosition = vec4(1,0,0,1);

in vec3 in_Vertex;
in vec3 in_Normal;
in vec3 in_Tangent;
in vec3 in_Bitangent;
in vec2 in_Uv;

out vec3 N, L, H, R, T, B;

void main()
{
    vec3 V, eyeDir;
    vec4 pos;
    pos = MVPMatrix * vec4(in_Vertex,1);
    eyeDir = pos.xyz;
    N  = normalize(NormalMatrix * in_Normal);
    L  = normalize(LightDir);
    V  = normalize((MVMatrix * ViewPosition).xyz - pos.xyz);
    H  = normalize(L + V);
    R  = normalize(reflect(eyeDir, N));
    T  = normalize(NormalMatrix * in_Tangent);
    B  = normalize(NormalMatrix * in_Bitangent);
    gl_Position = pos;
}

