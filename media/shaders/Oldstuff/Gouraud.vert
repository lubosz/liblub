void main()
{
	//gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	//gl_Position = ftransform();
	//gl_Position= gl_ModelViewProjectionMatrix* gl_Vertex;

	vec3 normal = normalize(gl_Normal);				// Falls nicht normiert 
	vec3 lightVec= gl_LightSource[0].position.xyz-gl_Vertex.xyz;// Vektor zur Lichtquelle 
	lightVec= normalize(lightVec);					// Normalisieren 
	float cosinus= dot(normal, lightVec);				// Empfängercosinus 
	gl_FrontColor= gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse * cosinus;// Farbe berechnen 
	gl_Position= ftransform();						// Transformation in kanonisches Volumen

}
