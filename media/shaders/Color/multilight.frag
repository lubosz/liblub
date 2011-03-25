#version 330 core

struct LightSource {
	vec4 position;
	vec4 diffuseColor;
	vec4 specularColor;
	vec4 direction; 
};

in vec4 positionView;
in vec3 normalView;
in vec3 normalWorld;

out vec4 finalColor;

uniform vec4 lightPositionView;
uniform vec4 diffuseMaterialColor;

uniform LightSourceBuffer {
	LightSource lightSources[10];
};

vec4 diffuseColor(float lambertTerm){
	return diffuseMaterialColor * lambertTerm;
}

void main(){ 
	finalColor = vec4(0);

	vec4 lightDirection = lightPositionView - positionView;
								
	vec3 L = normalize(lightDirection.xyz);	
	vec3 N = normalize(normalView);
	
	float lambertTerm = max( dot(N,L), 0.0);

	finalColor += diffuseColor(lambertTerm);
//	finalColor = vec4(normalWorld,1);
} 

