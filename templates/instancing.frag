#version 330 core

uniform sampler2D diffuse;
in vec2 uv;
/*
precision highp float;


in vec3 viewNormal;
in float viewDistance;
*/
out vec4 fragColor;
const vec4 white = vec4(1);

void main(void) {
    fragColor = texture(diffuse, uv);
    //fragColor =  vec4(2.0/viewDistance);
    fragColor.a = 0.5;
    //fragColor = white;
}
