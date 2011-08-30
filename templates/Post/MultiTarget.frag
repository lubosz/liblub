#define LIGHTS 5
{% extends "base.frag" %}

{% block linkage %}
in vec4 positionView;
in vec3 normalView;
in vec3 tangentView;
in vec2 uv;
out vec4 positionTarget;
out vec4 normalTarget;
out vec4 diffuseTarget;
out vec4 tangentTarget;
out vec4 normalMapTarget;
out vec4 envTarget;
out vec4 shadowTarget;

//out float depthTarget;
{% endblock %}

{% block uniforms %}
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform samplerCube envMap;
//uniform sampler2DShadow {{shadowDepthSource}};

{% for shadowSampler in shadowSamplers %}
uniform sampler2DShadow {{shadowSampler}};
uniform mat4 camViewToShadowMapMatrix{{shadowSampler}}; //bias*perspLight*viewLight*(viewCam⁻1)
{% endfor %}

{% endblock %}


{% block functions %}
/*
float xPixelOffset = 1.0/1366.0;
float yPixelOffset = 1.0/768.0;

float lookup( vec2 offSet,vec4 shadowTexCoord){
	// Values are multiplied by ShadowCoord.w because shadow2DProj does a W division for us.
	return textureProj( {{shadowDepthSource}}, 
		shadowTexCoord 
		+ vec4(
			offSet.x * xPixelOffset * shadowTexCoord.w, 
			offSet.y * yPixelOffset * shadowTexCoord.w, 
			0, 
			0
		) 
	);
}
*/
{% endblock %}

{% block main %}
	positionTarget = positionView;
	normalTarget = vec4(normalView,1);
	tangentTarget =  vec4(tangentView,1);
	diffuseTarget = texture(diffuseTexture, uv*5);
	normalMapTarget = texture(normalTexture, uv);
	vec3 reflectDir = reflect(-positionView.xyz, normalView);
	envTarget = texture(envMap, reflectDir);
	
	//shadow
	shadowTarget = vec4(1);
	vec4 shadowTexCoord;
	float shadowSum = 0;
{% for shadowSampler in shadowSamplers %}
	shadowTexCoord = camViewToShadowMapMatrix{{shadowSampler}} * positionView;
	shadowSum += textureProj({{shadowSampler}}, shadowTexCoord);
{% endfor %}
	
	shadowTarget*= shadowSum/5.0;

	/*
	// 8x8 kernel PCF
	vec4 shadowTexCoord = camViewToShadowMapMatrix * positionView;
	float shadow = 0;	
	float x,y;
	for (y = -3.5 ; y <=3.5 ; y+=1.0)
		for (x = -3.5 ; x <=3.5 ; x+=1.0)
			shadow += lookup(vec2(x,y),shadowTexCoord);
				
	shadow /= 32.0;
	*/
	//gl_FragDepth = positionView.z/20;
	//depthTarget = positionView.z/20;
{% endblock %}
