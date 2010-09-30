#version 330 core

//precision highp float;

in vec4 positionView;
in vec3 normalView;
in vec2 uv;

out vec4 finalColor;

uniform vec4 lightPositionView;

uniform vec4 lightColor;

//ambient
uniform vec4 ambientSceneColor;
//diffuse
uniform vec4 diffuseMaterialColor;
//specular
uniform float shininess;
uniform vec4 specularMaterialColor;

uniform sampler2D diffuseTexture;

vec4 diffuseColor(float lambertTerm){

	return	diffuseMaterialColor * 
			texture(diffuseTexture, uv) *
			lightColor * 
			lambertTerm;
}

vec4 specularColor(float specular){

	return	specularMaterialColor * 
			lightColor * 
			specular;	
}

void main(){ 

	vec4 lightDirection = lightPositionView - positionView;

	//ambient
	finalColor = ambientSceneColor;
								
	vec3 N = normalize(normalView);
	vec3 L = normalize(lightDirection.xyz);
	
	float lambertTerm = dot(N,L);

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
			
} 

