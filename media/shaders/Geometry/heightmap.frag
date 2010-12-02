#version 130

uniform sampler2D colorMap;

in vec2 uv;

//out vec4 fragColor;

void main(void) {
    gl_FragColor = texture(colorMap, uv);
}
