#version 330 core

precision highp float;
uniform sampler2D diffuse;

in vec2 uv;
in vec3 viewNormal;
out vec4 fragColor;

void main(void) {
    //fragColor = vec4(viewNormal,1.0);
    //fragColor = texture(diffuse, uv);
    fragColor = vec4(uv,1,1.0);
}
