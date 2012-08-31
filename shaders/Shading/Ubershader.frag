{% extends "base.frag" %}

{% block linkage %}
{% if viewposition %}
in vec4 positionView;
{% endif %}
{% if normal %}
in vec3 normalView;
{% endif %}
{% if uv %}
in vec2 uv;
{% endif %}
{% endblock %}

{% block uniform %}

{% if light %}
{% include "Shading/singlelight.frag" %}
{% endif %}

{% if diffuseTexture %}
uniform sampler2D diffuseTexture;
{% endif %}

{% if normalmap %}
{% include "Shading/normalmap.frag" %}
{% endif %}

{% if spotlight %}
{% include "Shading/spotlight.frag" %}
{% endif %}

{% if shadow %}
{% include "Shading/shadow.frag" %}
{% endif %}

{% endblock %}


{% block main %}
  fragColor = vec4(0);
  vec4 lightDirection = lightPositionView - positionView;
  {% comment %}float d = length(lightDirection);{% endcomment %}

{% if normalmap %}
		fragColor*= diffuseBump/2.5;
{% endif %}
	
	vec3 L = normalize(lightDirection.xyz);	

{% if spotlight %}
	//Spot	
	float spotCurAngle = dot(-L, spotDirectionView);
	float spotAngleDiff =  spotOuterAngle - spotInnerAngle;
	float spot = clamp((spotCurAngle - spotOuterAngle) / spotAngleDiff, 0.0, 1.0);
{% endif %}

	vec3 N = normalize(normalView);
	
	float lambertTerm = max( dot(N,L), 0.0);

	if(lambertTerm > 0.0)	{
		//diffuse
		fragColor += diffuseColor(lambertTerm)
{% if normalmap %}
		* diffuseBump
{% endif %}
;
{% if shadow %}
    float shadow = getShadow();
		if (shadow > 0){
{% endif %}	
		//specular
{% if normalmap %}
		fragColor += specularColor(specular);
{% else %}	
  fragColor += specularColor(L,N);
{% endif %}	

		
{% if shadow %}
		}
{% endif %}	
	}
	
{% if attenuation %}
	fragColor *= att;
{% endif %}	
	
{% if spotlight %}
	fragColor *= spot
{% endif %}	
	
{% if shadow %}
	fragColor *= shadow;
{% endif %}

//set opaque
	fragColor.w = 1.0;
{% endblock %}
