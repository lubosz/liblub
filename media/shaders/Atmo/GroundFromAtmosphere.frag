//
// Atmospheric scattering fragment shader
//
// Author: Sean O'Neil
//
// Copyright (c) 2004 Sean O'Neil
//

//uniform sampler2D s2Tex1;
//uniform sampler2D s2Tex2;
#version 410 core

//uniform sampler2D glow;

//in vec2 uv;
in vec3 color1;
in vec3 color2;
out vec4 fragColor;

void main (void)
{
	//gl_FragColor = gl_Color + 0.25 * gl_SecondaryColor;
	fragColor = vec4(color1 + 0.25 * color2,1);
	//gl_FragColor = gl_Color + texture2D(s2Tex1, gl_TexCoord[0].st) * texture2D(s2Tex2, gl_TexCoord[1].st) * gl_SecondaryColor;
}
