//
// Atmospheric scattering fragment shader
//
// Author: Sean O'Neil
//
// Copyright (c) 2004 Sean O'Neil
//
#version 410 core

uniform sampler2D glow;

in vec2 uv;
in vec3 color;

out vec4 fragColor;

void main (void) {
	fragColor = vec4(color,1) *texture(glow, uv);
}
