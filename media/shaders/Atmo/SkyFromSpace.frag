//
// Atmospheric scattering fragment shader
//
// Author: Sean O'Neil
//
// Copyright (c) 2004 Sean O'Neil
//
#version 410 core
precision highp float;

uniform vec3 v3LightPos;
uniform float g;
uniform float g2;

in vec3 v3Direction;
in vec3 color1;
in vec3 color2;

out vec4 fragColor;

void main (void)
{
	float fCos = dot(v3LightPos, v3Direction) / length(v3Direction);
	float fMiePhase = 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos*fCos) / pow(1.0 + g2 - 2.0*g*fCos, 1.5);
    fragColor = vec4(color1 + fMiePhase * color2,1);
    //fragColor = vec4(0) + fMiePhase;
	fragColor.a = fragColor.b;
	//fragColor.w = fragColor.y;
}
