#version 330 core

struct LightSource {
	vec4 position;
	vec4 diffuse;
	vec4 specular;
	vec4 direction; 
};

in vec4 positionView;
in vec3 normalView;

out vec4 finalColor;
out vec4 normalColor;

uniform vec4 lightPositionView;
uniform float shininess;
uniform vec4 specularMaterialColor;

in vec2 uv;
uniform sampler2D diffuseTexture;
uniform vec4 diffuseMaterialColor;

in vec3 tangentView;
uniform sampler2D normalTexture;

uniform samplerCube envMap;

uniform sampler2DShadow shadowMap;
uniform mat4 camViewToShadowMapMatrix; //bias*perspLight*viewLight*(viewCam⁻1)

float xPixelOffset = 1.0/1366.0;
float yPixelOffset = 1.0/768.0;

float lookup( vec2 offSet,vec4 shadowTexCoord){
	// Values are multiplied by ShadowCoord.w because shadow2DProj does a W division for us.
	return textureProj(shadowMap, 
		shadowTexCoord 
		+ vec4(
			offSet.x * xPixelOffset * shadowTexCoord.w, 
			offSet.y * yPixelOffset * shadowTexCoord.w, 
			0, 
			0
		) 
	);
	/*if (shadow == 1)
		return 1.0;
	else
		return 0.0;
		*/
}

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

	//env	
	vec3 reflectDir = reflect(-positionView.xyz, normalView);
	vec4 reflection = texture(envMap, reflectDir);

	//shadow	
	//vec4 shadowTexCoord = camViewToShadowMapMatrix * positionView;
	//float shadow = textureProj(shadowMap, shadowTexCoord);
	
	// 8x8 kernel PCF
	vec4 shadowTexCoord = camViewToShadowMapMatrix * positionView;
	float shadow = 0;	
	float x,y;
	for (y = -3.5 ; y <=3.5 ; y+=1.0)
		for (x = -3.5 ; x <=3.5 ; x+=1.0)
			shadow += lookup(vec2(x,y),shadowTexCoord);
				
	//shadow /= 64.0;
	shadow /= 32.0;
	//shadow += 0.2;

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
		finalColor += specularColor(specular) * lightSources[i].specular * reflection;
		
	}
	
		//finalColor = vec4(1) * shadow;
		finalColor *= shadow;
	//finalColor = texture(envMap, reflectDir);
	//finalColor = vec4(1) * shadow;
	//finalColor = lightSources[0].diffuse;
	//finalColor = texture(normalTexture, uv);
	//finalColor = vec4(normalView,1);
	normalColor = vec4(normalView,1);
	
} 

