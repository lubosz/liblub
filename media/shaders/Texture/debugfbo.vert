#version 330 core

uniform mat4 MVPMatrix;

in vec3 in_Vertex;
in vec3 in_Normal;
in vec3 in_Tangent;
in vec3 in_Bitangent;
in vec2 in_Uv;

out vec3 position;
//out vec3 normal;
out vec2 uv;

void main()
{

    position = in_Vertex;
  //  normal = in_Normal;
    uv = in_Uv;
    gl_Position = MVPMatrix * vec4(in_Vertex,1);
}

