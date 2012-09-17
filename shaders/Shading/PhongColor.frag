#version {{ version }}

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

{% if useDiffuseTexture %}
in vec2 uv;
uniform sampler2D diffuseTexture;
{% else %}
uniform vec4 diffuseMaterialColor;
{% endif %}

{% if useNormalTexture %}
in vec3 lightVec;
in vec3 eyeVec;
uniform sampler2D normalTexture;
{% endif %}

{% if useAmbientTexture %}
uniform sampler2D ambientTexture;
{% endif %}

{% if useSpotLight %}
//spot
uniform float spotInnerAngle;
uniform float spotOuterAngle;
uniform vec3 spotDirectionView;
{% endif %}

{% if receiveShadows %}
uniform sampler2DShadow shadowMap;
uniform mat4 camViewToShadowMapMatrixshadowDepthSource0; //bias*perspLight*viewLight*(viewCam⁻1)
{% endif %}

{% if usePCF %}
// This define the value to move one pixel left or right
uniform float xPixelOffset ;
// This define the value to move one pixel up or down
uniform float yPixelOffset ;
{% endif %}

//attenuation
uniform float constantAttenuation;
uniform float linearAttenuation;
uniform float quadraticAttenuation;

{% if usePCF %}
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
{% endif %}


vec4 diffuseColor(float lambertTerm){

	return	
{% if useDiffuseTexture %}
		texture(diffuseTexture, uv) *
{% else %}
		diffuseMaterialColor *
{% endif %}
			lightColor * 
			lambertTerm;
}

vec4 specularColor(float specular){

	return	specularMaterialColor * 
			lightColor * 
			specular;	
}

void main(){ 


{% if useNormalTexture %}
	//vec3 lVec = lightVec * inversesqrt(dot(lightVec, lightVec));
	vec3 lVec = normalize(lightVec);
	vec3 bump = normalize( texture(normalTexture, uv).xyz * 2.0 - 1.0);

	float diffuseBump = max( dot(lVec, bump), 0.0 );
{% endif %}

	vec4 lightDirection = lightPositionView - positionView;

	float d = length(lightDirection);
		
	float att = 1.0 / ( 
			constantAttenuation + 
			(linearAttenuation * d) + 
			(quadraticAttenuation * d * d) 
	);

	//ambient
{% if useAmbientTexture %}
	finalColor = texture(ambientTexture,uv)*ambientSceneColor;
{% else %}
	finalColor = ambientSceneColor;
{% endif %}

{% if useNormalTexture %}
		finalColor*= diffuseBump/2.5;
{% endif %}
	
	vec3 L = normalize(lightDirection.xyz);	

{% if usePCF %}
// 8x8 kernel PCF
	vec4 shadowTexCoord = camViewToShadowMapMatrixshadowDepthSource0 * positionView;
	float shadow = 0;	
	float x,y;
	for (y = -3.5 ; y <=3.5 ; y+=1.0)
		for (x = -3.5 ; x <=3.5 ; x+=1.0)
			shadow += lookup(vec2(x,y),shadowTexCoord);
				
	shadow /= 64.0;
	shadow += 0.2;
{% else %}{% if receiveShadows %}
	vec4 shadowTexCoord = camViewToShadowMapMatrixshadowDepthSource0 * positionView;
	float shadow = textureProj(shadowMap, shadowTexCoord);
{% endif %}{% endif %}

{% if useSpotLight %}
	//Spot	
	float spotCurAngle = dot(-L, spotDirectionView);
	float spotAngleDiff =  spotOuterAngle - spotInnerAngle;
	float spot = clamp((spotCurAngle - spotOuterAngle) / spotAngleDiff, 0.0, 1.0);
{% endif %}

	vec3 N = normalize(normalView);
	
	//float lambertTerm = dot(N,L);

	float lambertTerm = max( dot(N,L), 0.0);

	if(lambertTerm > 0.0)
	{
		//diffuse
		finalColor += diffuseColor(lambertTerm)
{% if useSpotLight %}
		* spot
{% endif %}
{% if useNormalTexture %}
		* diffuseBump
{% endif %}
		* att;

{% if receiveShadows %}
		if (shadow > 0){
{% endif %}	
		//specular
{% if useNormalTexture %}
		float specular = pow(
						clamp(
							dot(
								reflect(-lVec, bump), 
								normalize(eyeVec)
							), 
							0.0, 
							1.0
						), shininess );
{% else %}
		vec3 E = normalize(-positionView.xyz);
		vec3 R = reflect(-L, N);

		float specular = pow( max(dot(R, E), 0.0), shininess );
{% endif %}

		
		finalColor += specularColor(specular)
		
{% if useSpotLight %}
		* spot
{% endif %}
		* att;
{% if receiveShadows %}
		}
{% endif %}	
	}
	
{% if receiveShadows %}
	finalColor *= shadow;
{% endif %}
} 

