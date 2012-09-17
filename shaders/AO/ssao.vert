#version {{ version }}

uniform mat4 MVPMatrix;

in vec3 in_Vertex;
in vec2 in_Uv;

out vec3 position;
out vec2 uv;

void main() {
    position = in_Vertex;
    uv = in_Uv;
    gl_Position = MVPMatrix * vec4(in_Vertex,1);
}
