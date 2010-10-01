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

//spot
uniform float spotCosCutoff;
uniform vec3 spotDirectionView;

//attenuation
uniform float constantAttenuation;
uniform float linearAttenuation;
uniform float quadraticAttenuation;

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

	float d = length(lightDirection);
		
	float att = 1.0 / ( 
			constantAttenuation + 
			(linearAttenuation * d) + 
			(quadraticAttenuation * d * d) 
	);


	//ambient
	finalColor = ambientSceneColor;
								
	vec3 L = normalize(lightDirection.xyz);

	vec3 D = normalize(spotDirectionView);

if (dot(-L, spotDirectionView) > spotCosCutoff){

	vec3 N = normalize(normalView);
	
	//float lambertTerm = dot(N,L);

	float lambertTerm = max( dot(N,L), 0.0);

	if(lambertTerm > 0.0)
	{
		//diffuse
		finalColor += diffuseColor(lambertTerm) * att;
		
		//specular
		vec3 E = normalize(-positionView.xyz);
		vec3 R = reflect(-L, N);

		float specular = pow( max(dot(R, E), 0.0), shininess );
		finalColor += specularColor(specular) * att;
	}
}
			
} 

