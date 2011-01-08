#version 330 core

uniform sampler2D color;
uniform sampler2D height;
uniform sampler2D normal;

in vec3 in_Vertex;
in vec2 in_Uv;

out vec2 uv;

uniform mat4 MVPMatrix;
uniform vec2 uvoffset;

uniform float tiling;

void main(void) {
//    uv = (in_Uv + uvoffset)*tiling;
      uv = (in_Uv + uvoffset);  
    float h = texture(height, uv).x;
    vec4 p = vec4(in_Vertex,1);
    p.y += h/4.0;

    gl_Position = MVPMatrix * p;
}
