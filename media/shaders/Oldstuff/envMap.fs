uniform samplerCube tex;
varying vec3 r;

void main()
{
	//gl_FragColor = gl_Color * textureCube(tex, gl_TexCoord[0].xyz);
	gl_FragColor = gl_Color * textureCube(tex,r);
}
