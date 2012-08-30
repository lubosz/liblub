in vec3 lightVec;
in vec3 eyeVec;
uniform sampler2D normalTexture;

void normalMap() {
	//vec3 lVec = lightVec * inversesqrt(dot(lightVec, lightVec));
	vec3 lVec = normalize(lightVec);
	vec3 bump = normalize( texture(normalTexture, uv).xyz * 2.0 - 1.0);

	float diffuseBump = max( dot(lVec, bump), 0.0 );
}

float getSpecularBump() {
		return pow(
						clamp(
							dot(
								reflect(-lVec, bump), 
								normalize(eyeVec)
							), 
							0.0, 
							1.0
						), shininess );
}
