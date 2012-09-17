#version {{ version }}

precision highp float;

in  vec3 geom_Color;

out vec4 fragColor;

void main(void) {
    fragColor = vec4(geom_Color,1.0);
}
