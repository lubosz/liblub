// BillboardFrag.glsl
//

uniform sampler2DRect s2Tex1;

void main()
{
	gl_FragColor = gl_Color * texture2DRect(s2Tex1, gl_TexCoord[0].st);
}

