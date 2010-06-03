#version 150 core

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;
uniform vec3 LightDir; // Light direction in eye coordinates
uniform vec4 ViewPosition;

in    vec4  MCvertex;
in    vec3  MCnormal;
in    vec3  MCTangent;
in    vec3  MCBinormal;

out   vec3  N, L, H, R, T, B;

void main()
{
    vec3 V, eyeDir;
    vec4 pos;
    pos     = MVPMatrix * MCVertex;
    eyeDir = pos.xyz;
    N  = normalize(NormalMatrix * MCNormal);
    L  = normalize(LightDir);
    V  = normalize((MVMatrix * ViewPosition).xyz - pos.xyz);
    H  = normalize(L + V);
    R  = normalize(reflect(eyeDir, N));
    T  = normalize(NormalMatrix * MCTangent);
    B  = normalize(NormalMatrix * MCBinormal);
    gl_Position = MVPMatrix * MCVertex;
}

