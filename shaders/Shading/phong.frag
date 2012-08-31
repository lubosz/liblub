//ambient
uniform vec4 ambientSceneColor;
//specular
uniform float shininess;
uniform vec4 specularMaterialColor;
uniform vec4 diffuseMaterialColor;

vec4 diffuseColor(float lambertTerm){

	return	
#ifdef useDiffuseTexture
		texture(diffuseTexture, uv) *
#endif
		diffuseMaterialColor *
			lightColor * 
			lambertTerm;
}

vec4 specularColor(vec3 L, vec3 N){
		vec3 E = normalize(-positionView.xyz);
		vec3 R = reflect(-L, N);

		float specular = pow( max(dot(R, E), 0.0), shininess );

	return	specularMaterialColor * 
			lightColor * 
			specular;	
}
