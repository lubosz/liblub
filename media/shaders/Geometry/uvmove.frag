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
uniform float xPixelOffset;
uniform float yPixelOffset;

//shadow
uniform sampler2DShadow shadowMap;
uniform mat4 camViewToShadowMapMatrix; //bias*perspLight*viewLight*(viewCam⁻1)

//normalmap
in vec3 lightVec;
in vec3 eyeVec;
uniform sampler2D normalTexture;


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
}

void main(void) {
// 8x8 kernel PCF
	vec4 shadowTexCoord = camViewToShadowMapMatrix * positionView;
	float shadow = 0;	
	float x,y;
	for (y = -3.5 ; y <=3.5 ; y+=1.0)
		for (x = -3.5 ; x <=3.5 ; x+=1.0)
			shadow += lookup(vec2(x,y),shadowTexCoord);
				
	shadow /= 64.0;
	shadow += 0.2;

//normalmap
	vec3 lVec = normalize(lightVec);
	vec3 bump = normalize( texture(normalTexture, uv).xyz * 2.0 - 1.0);

	float diffuseBump = max( dot(lVec, bump), 0.0 );
//

	vec4 lightDirection = lightPositionView - positionView;

	vec3 L = normalize(lightDirection.xyz);
	vec3 N = normalize(normalView);
	

	float lambertTerm = max( dot(N,L), 0.0);

	finalColor = vec4(0,0,0,1);
	if(lambertTerm > 0.0)
	{
		//diffuse
		finalColor += diffuseColor(lambertTerm)* diffuseBump;
		//if (shadow > 0){
			//specular
			//vec3 E = normalize(-positionView.xyz);
			//vec3 R = reflect(-L, N);

			//float specular = pow( max(dot(R, E), 0.0), shininess );
			//normal spec
					float specular = pow(
						clamp(
							dot(
								reflect(-lVec, bump), 
								normalize(eyeVec)
							), 
							0.0, 
							1.0
						), shininess );

		
			finalColor += specularColor(specular);
		//}
	}
	
	finalColor *= shadow;


	//fragColor = normalColor;
	//fragColor = vec4(normalView, 1);
    //finalColor = texture(color, uv);
    //finalColor = vec4(N,1);
}
