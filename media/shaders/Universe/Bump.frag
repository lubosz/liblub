// BumpFrag.glsl
//

uniform vec3 v3LightPos;		// The direction vector to the light source
uniform sampler2D s2Tex;

void main()
{
	vec3 v3Normal = texture2D(s2Tex, gl_TexCoord[1].xy).xyz - 0.5;
	float fBump = dot(v3LightPos, v3Normal) / length(v3Normal);
	gl_FragColor = fBump * texture2D(s2Tex, gl_TexCoord[0].xy);
}

