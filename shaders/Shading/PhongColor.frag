#version 330 core

#define useDiffuseTexture 1
#define useNormalTexture 1
#define useSpotLight 1
#define receiveShadows 1
#define usePCF 1

precision highp float;

in vec4 positionView;
in vec3 normalView;

out vec4 finalColor;

uniform vec4 lightPositionView;

uniform vec4 lightColor;

//ambient
uniform vec4 ambientSceneColor;
//specular
uniform float shininess;
uniform vec4 specularMaterialColor;

#ifdef useDiffuseTexture
in vec2 uv;
uniform sampler2D diffuseTexture;
#else
uniform vec4 diffuseMaterialColor;
#endif

#ifdef useNormalTexture 
in vec3 lightVec;
in vec3 eyeVec;
uniform sampler2D normalTexture;
#endif

#ifdef useAmbientTexture
uniform sampler2D ambientTexture;
#endif

#ifdef useSpotLight
//spot
uniform float spotInnerAngle;
uniform float spotOuterAngle;
uniform vec3 spotDirectionView;
#endif

#ifdef receiveShadows
uniform sampler2DShadow shadowMap;
uniform mat4 camViewToShadowMapMatrixshadowDepthSource0; //bias*perspLight*viewLight*(viewCam⁻1)
#endif

#ifdef usePCF
// This define the value to move one pixel left or right
uniform float xPixelOffset ;
// This define the value to move one pixel up or down
uniform float yPixelOffset ;
#endif

//attenuation
uniform float constantAttenuation;
uniform float linearAttenuation;
uniform float quadraticAttenuation;

#ifdef usePCF
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
#endif


vec4 diffuseColor(float lambertTerm){

	return	
#ifdef useDiffuseTexture
		texture(diffuseTexture, uv) *
#else
		diffuseMaterialColor *
#endif
			lightColor * 
			lambertTerm;
}

vec4 specularColor(float specular){

	return	specularMaterialColor * 
			lightColor * 
			specular;	
}

void main(){ 


#ifdef useNormalTexture 
	//vec3 lVec = lightVec * inversesqrt(dot(lightVec, lightVec));
	vec3 lVec = normalize(lightVec);
	vec3 bump = normalize( texture(normalTexture, uv).xyz * 2.0 - 1.0);

	float diffuseBump = max( dot(lVec, bump), 0.0 );
#endif

	vec4 lightDirection = lightPositionView - positionView;

	float d = length(lightDirection);
		
	float att = 1.0 / ( 
			constantAttenuation + 
			(linearAttenuation * d) + 
			(quadraticAttenuation * d * d) 
	);

	//ambient
#ifdef useAmbientTexture
	finalColor = texture(ambientTexture,uv)*ambientSceneColor;
#else
	finalColor = ambientSceneColor;
#endif

#ifdef useNormalTexture 
		finalColor*= diffuseBump/2.5;
#endif
	
								
	vec3 L = normalize(lightDirection.xyz);	
	


#ifdef usePCF
// 8x8 kernel PCF
	vec4 shadowTexCoord = camViewToShadowMapMatrixshadowDepthSource0 * positionView;
	float shadow = 0;	
	float x,y;
	for (y = -3.5 ; y <=3.5 ; y+=1.0)
		for (x = -3.5 ; x <=3.5 ; x+=1.0)
			shadow += lookup(vec2(x,y),shadowTexCoord);
				
	shadow /= 64.0;
	shadow += 0.2;
#else
	#ifdef receiveShadows
		vec4 shadowTexCoord = camViewToShadowMapMatrixshadowDepthSource0 * positionView;
		float shadow = textureProj(shadowMap, shadowTexCoord);
	#endif	
#endif

#ifdef useSpotLight
	//Spot	
	float spotCurAngle = dot(-L, spotDirectionView);
	float spotAngleDiff =  spotOuterAngle - spotInnerAngle;
	float spot = clamp((spotCurAngle - spotOuterAngle) / spotAngleDiff, 0.0, 1.0);
#endif

	vec3 N = normalize(normalView);
	
	//float lambertTerm = dot(N,L);

	float lambertTerm = max( dot(N,L), 0.0);

	if(lambertTerm > 0.0)
	{
		//diffuse
		finalColor += diffuseColor(lambertTerm)
#ifdef useSpotLight
		* spot
#endif
#ifdef useNormalTexture 
		* diffuseBump
#endif
		* att;

#ifdef receiveShadows
		if (shadow > 0){
#endif	
		//specular
#ifdef useNormalTexture 
		float specular = pow(
						clamp(
							dot(
								reflect(-lVec, bump), 
								normalize(eyeVec)
							), 
							0.0, 
							1.0
						), shininess );
#else
		vec3 E = normalize(-positionView.xyz);
		vec3 R = reflect(-L, N);

		float specular = pow( max(dot(R, E), 0.0), shininess );
#endif

		
		finalColor += specularColor(specular)
		
#ifdef useSpotLight
		* spot
#endif
		* att;
#ifdef receiveShadows
		}
#endif	
	}
	
#ifdef receiveShadows
	finalColor *= shadow;
#endif
} 

