#version 330 core

uniform sampler2D normalTarget;

in vec2 uv;

out vec4 fragColor;

void main() {
	fragColor = texture(normalTarget, uv);
}
