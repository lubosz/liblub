//
// Atmospheric scattering fragment shader
//
// Author: Sean O'Neil
//
// Copyright (c) 2004 Sean O'Neil
//
#version 410 core

uniform vec3 v3LightPos;
uniform float g;
uniform float g2;

in vec3 v3Direction;

out vec4 fragColor;

void main (void)
{
	float fCos = dot(v3LightPos, v3Direction) / length(v3Direction);
	float fMiePhase = 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos*fCos) / pow(1.0 + g2 - 2.0*g*fCos, 1.5);
    //fragColor = gl_Color + fMiePhase * gl_SecondaryColor;
    fragColor = vec4(0) + fMiePhase;
	fragColor.a = fragColor.b;
}
