{% extends "base.frag" %}

{% block linkage %}
in vec4 positionView;
//in vec3 normalView;
in vec2 uv;
in vec4 positionWorld;
in vec4 normalWorld;
in vec4 tangentWorld;
out vec4 positionTarget;
out vec4 normalTarget;
out vec4 diffuseTarget;
out vec4 tangentTarget;
out vec4 normalMapTarget;
out vec4 shadowTarget;
{% endblock %}

{% block uniforms %}
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform samplerCube envMap;

{% for shadowSampler in shadowSamplers %}
uniform sampler2DShadow {{shadowSampler}};
//bias*perspLight*viewLight*(viewCam^-1)
uniform mat4 camViewToShadowMapMatrix{{shadowSampler}};
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
	positionTarget = positionWorld;
	normalTarget = normalWorld;
	tangentTarget = tangentWorld;
	diffuseTarget = texture(diffuseTexture, uv);
	normalMapTarget = texture(normalTexture, uv);
	
	//shadow
	shadowTarget = vec4(1);
	vec4 shadowTexCoord;
	float shadowSum = 0;
{% for shadowSampler in shadowSamplers %}
	shadowTexCoord = camViewToShadowMapMatrix{{shadowSampler}} * positionView;
	shadowSum += textureProj({{shadowSampler}}, shadowTexCoord);
{% endfor %}
	
	shadowTarget*= shadowSum/{{shadowSamplerSize}}.0;

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
{% endblock %}
