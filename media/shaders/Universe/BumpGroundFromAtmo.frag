//
// Atmospheric scattering fragment shader
//
// Author: Sean O'Neil
//
// Copyright (c) 2004 Sean O'Neil
//

uniform vec3 v3LightPos;		// The direction vector to the light source
uniform sampler2D s2Tex;

void main (void)
{
	vec3 v3Normal = texture2D(s2Tex, gl_TexCoord[1].xy).xyz - 0.5;
	float fBump = sqrt(max(0.01, dot(v3LightPos, v3Normal) / length(v3Normal)));
	gl_FragColor = gl_Color + gl_SecondaryColor * fBump * texture2D(s2Tex, gl_TexCoord[0].xy);
}
