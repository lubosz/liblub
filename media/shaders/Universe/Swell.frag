// SwellFrag.glsl
//

uniform sampler3D s3Tex0;

void main()
{
/*
	gl_FragColor.r = gl_Color.r * texture3D(s3Tex0, gl_TexCoord[0].xyz).a;
	gl_FragColor.g = gl_Color.g * texture3D(s3Tex0, gl_TexCoord[1].xyz).a;
	gl_FragColor.b = gl_Color.b * texture3D(s3Tex0, gl_TexCoord[2].xyz).a;
	gl_FragColor.a = 1.0;
*/
	gl_FragColor = gl_Color * (
		0.5 * texture3D(s3Tex0, gl_TexCoord[0].xyz) +
		0.25 * texture3D(s3Tex0, gl_TexCoord[1].xyz) +
		0.125 * texture3D(s3Tex0, gl_TexCoord[2].xyz) +
		0.0625 * texture3D(s3Tex0, gl_TexCoord[3].xyz));
}

