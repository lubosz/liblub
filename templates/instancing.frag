#version 330 core

precision highp float;
uniform sampler2D diffuse;

in vec2 uv;
in vec3 viewNormal;
out vec4 fragColor;

void main(void) {
    fragColor = texture(diffuse, uv);
}
