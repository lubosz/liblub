#version 330 core

uniform sampler2D color;

in vec2 uv;
in vec4 positionView;
in float dist;
out vec4 fragColor;

void main(void) {
//    fragColor = texture(color, uv);
//fragColor = positionView;
fragColor = vec4(dist/10.0);

}
