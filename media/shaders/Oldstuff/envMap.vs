varying vec3 r;

void main()
{
	gl_FrontColor = gl_Color;
	//gl_TexCoord[0] = gl_MultiTexCoord0;

	vec4 ecPosition = gl_ModelViewMatrix * gl_Vertex;
	vec3 ecPosition3 = ecPosition.xyz / ecPosition.w;

	vec3 i = normalize(ecPosition3);
	vec3 n = normalize(gl_NormalMatrix * gl_Normal);
	
	//r = reflect(i, n);
	r = refract(i, n, 1.1);
	gl_Position = ftransform();	

}
