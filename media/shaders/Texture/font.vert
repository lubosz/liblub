#version 330 core

uniform mat4 MVPMatrix;

in vec3 in_Vertex;
in vec3 in_Normal;
in vec3 in_Tangent;
in vec3 in_Binormal;
in vec2 in_Uv;

out vec2 uv;

void main()
{

    uv = vec2(in_Uv.x, -in_Uv.y);
    gl_Position = MVPMatrix * vec4(in_Vertex,1);
}

