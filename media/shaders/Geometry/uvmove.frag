#version 330 core

uniform sampler2D color;

in vec2 uv;
in vec3 normalView;
in vec4 positionView;

out vec4 finalColor;

uniform vec4 lightPositionView;

uniform vec4 lightColor;

//specular
uniform float shininess;
uniform vec4 specularMaterialColor;

vec4 diffuseColor(float lambertTerm){

	return	
		texture(color, uv) *
		//vec4(0,0,1,1) *
			lightColor * 
			lambertTerm;
}

vec4 specularColor(float specular){

	return	specularMaterialColor * 
			lightColor * 
			specular;	
}

void main(void) {
	vec4 lightDirection = lightPositionView - positionView;

	vec3 L = normalize(lightDirection.xyz);
	vec3 N = normalize(normalView);
	

	float lambertTerm = max( dot(N,L), 0.0);

	finalColor = vec4(0);
	if(lambertTerm > 0.0)
	{
		//diffuse
		finalColor += diffuseColor(lambertTerm);

		//specular
		vec3 E = normalize(-positionView.xyz);
		vec3 R = reflect(-L, N);

		float specular = pow( max(dot(R, E), 0.0), shininess );

		
		finalColor += specularColor(specular);
	}


	//fragColor = normalColor;
	//fragColor = vec4(normalView, 1);
    finalColor = texture(color, uv);
}
