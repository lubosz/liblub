#version 330

uniform mat4 PMatrix;
uniform mat4 MVMatrix;

in vec3 in_Vertex;
//in vec3 in_Color;
in vec3 in_Normal;
//in vec2 in_Uv;

out vec3 position;
out vec3 normal;
//out vec3 color;
//out vec2 uv;

void main()
{
    vec4 eye_position = MVMatrix * vec4(in_Vertex, 1.0);
    gl_Position = PMatrix * eye_position;
    position = eye_position.xyz;
    //normal   = (MVMatrix * vec4(in_Normal, 0.0)).xyz;
    normal = in_Normal;
    //uv = in_Uv;
    //color = in_Color;
}
