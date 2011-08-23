#version 330 core

uniform sampler2D result;

in vec2 uv;

out vec4 fragColor;

void main() {
	fragColor = texture(result, uv);
}
