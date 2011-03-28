#version 330 core

struct LightSource {
	vec4 position;
	vec4 diffuse;
	vec4 specular;
	vec4 direction; 
};

in vec4 positionView;
in vec3 normalView;
in vec3 normalWorld;

out vec4 finalColor;

uniform vec4 lightPositionView;
uniform vec4 diffuseMaterialColor;

uniform LightSourceBuffer {
	LightSource lightSources[5];
};

vec4 diffuseColor(float lambertTerm){
	return diffuseMaterialColor * lambertTerm;
}

void main(){ 
	finalColor = vec4(0);

	for(int i = 0; i < 5 ; i++) {
		vec4 lightDirection = lightSources[i].position - positionView;
								
		vec3 L = normalize(lightDirection.xyz);	
		vec3 N = normalize(normalView);
	
		float lambertTerm = max( dot(N,L), 0.0);

		finalColor += diffuseColor(lambertTerm) * lightSources[i].diffuse;

	}
	//finalColor = lightSources[0].diffuse;
} 

