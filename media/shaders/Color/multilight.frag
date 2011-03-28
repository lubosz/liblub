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
uniform float shininess;
uniform vec4 specularMaterialColor;

in vec2 uv;
uniform sampler2D diffuseTexture;
uniform vec4 diffuseMaterialColor;

in vec3 tangentView;
uniform sampler2D normalTexture;

uniform LightSourceBuffer {
	LightSource lightSources[5];
};

vec4 diffuseColor(float lambertTerm){
	return texture(diffuseTexture, uv) * diffuseMaterialColor * lambertTerm;
}

vec4 specularColor(float specular){
	return	specularMaterialColor * specular;	
}

void main(){ 
	finalColor = vec4(0);

	//normalmapping
	vec3 binormalView = cross(normalView, tangentView);

	vec3 eyeVec = vec3(
		dot(-positionView.xyz, tangentView),
		dot(-positionView.xyz, binormalView),
		dot(-positionView.xyz, normalView)
	);
	vec3 bump = normalize( texture(normalTexture, uv).xyz * 2.0 - 1.0);
	//endnormalmapping
	
	

	for(int i = 0; i < 5 ; i++) {
		vec4 lightDirection = lightSources[i].position - positionView;
		
		
		//normalmapping
		vec3 lightVec = normalize(vec3(
			dot(lightDirection.xyz, tangentView),
			dot(lightDirection.xyz, binormalView),
			dot(lightDirection.xyz, normalView)
		));
		float diffuseBump = max( dot(lightVec, bump), 0.0 );
		//endnormalmapping
								
		vec3 L = normalize(lightDirection.xyz);	
		vec3 N = normalize(normalView);
	
		float lambertTerm = max( dot(N,L), 0.0);

		finalColor += diffuseColor(lambertTerm) * lightSources[i].diffuse * diffuseBump;
		
		float specular = pow(clamp(dot(
								reflect(-lightVec, bump), 
								normalize(eyeVec)
							), 
							0.0, 
							1.0
						), shininess );
		/*
		vec3 E = normalize(-positionView.xyz);
		vec3 R = reflect(-L, N);

		float specular = pow( max(dot(R, E), 0.0), shininess );
		*/
		finalColor += specularColor(specular) * lightSources[i].specular;
		
	}
	//finalColor = lightSources[0].diffuse;
	//finalColor = texture(normalTexture, uv);
} 

