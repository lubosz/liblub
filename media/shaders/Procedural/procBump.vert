#version 150 core

uniform vec3 LightPosition;
uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;

in  vec3 in_Vertex;
in  vec3 in_Normal;
//in  vec3 in_Tangent;
in  vec2 in_Uv;

out vec3 LightDir;
out vec3 EyeDir;
out vec2 TexCoord;

void main()
{
    gl_Position    = MVPMatrix * vec4(in_Vertex,1);
    EyeDir         = vec3(MVMatrix * vec4(in_Vertex,1));
    TexCoord       = in_Uv.st;
    vec3 n = normalize(NormalMatrix * in_Normal);
    vec3 t  = NormalMatrix * vec3(in_Uv,1);	//tangent;
    //vec3 t = normalize(NormalMatrix * in_Tangent);
    vec3 b = cross(n, t);
    vec3 v;
    v.x = dot(LightPosition, t);
    v.y = dot(LightPosition, b);
    v.z = dot(LightPosition, n);
    LightDir = normalize(v);
    v.x = dot(EyeDir, t);
    v.y = dot(EyeDir, b);
    v.z = dot(EyeDir, n);
    EyeDir = normalize(v);
}

