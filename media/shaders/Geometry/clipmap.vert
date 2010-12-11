#version 330 core

uniform sampler2D heightMap;

in vec3 in_Vertex;
in vec3 in_Normal;
in vec3 in_Tangent;
in vec2 in_Uv;

out vec2 uv;

uniform mat4 MVPMatrix;

void main(void) {
    uv = in_Uv;
    vec4 height = texture(heightMap, uv);
    vec3 position = in_Vertex;
    position.y += mix(height.x,height.y,height.z);
    //float heightScale = mix(height.x,height.y,height.z);
    //position += in_Normal * heightScale * 0.2;
    //position -= in_Normal * 0.2;
    //position += normalize(in_Normal * heightScale) * 0.2;
    gl_Position = MVPMatrix * vec4(position,1);
}
