{% extends "base.frag" %}

{% block linkage %}
in vec2 uv;
struct LightSource {
	vec4 position;
	vec4 diffuse;
	vec4 specular;
	vec4 direction; 
};
{% endblock %}

{% block uniforms %}
uniform sampler2D positionTarget;
uniform sampler2D normalTarget;
uniform sampler2D diffuseTarget;
uniform sampler2D tangentTarget;
uniform sampler2D normalMapTarget;
uniform sampler2D envTarget;
uniform sampler2D finalAOTarget;
uniform LightSourceBuffer {
	LightSource lightSources[5];
};

uniform vec4 camPosition;

const int shininess = 32;

float saturate(float input) {
	return clamp(input, 0.0, 1.0); 
}

{% endblock %}

{% block main %}
	fragColor = texture(finalAOTarget, uv);
	for(int i = 0; i < 5 ; i++) {
		vec4 lightDirection = lightSources[i].position - texture(positionTarget, uv);
		vec4 L = normalize(lightDirection);	
		vec4 N = normalize(texture(normalTarget, uv));
		float lambertTerm = max( dot(N,L), 0.0);
		fragColor += lambertTerm * lightSources[i].diffuse;
		
		vec4 viewDirection = camPosition - texture(positionTarget, uv);
		vec4 E = normalize(-viewDirection);
		vec4 R = reflect(-L, N);

		float specular = pow( max(dot(R, E), 0.0), shininess );
		//fragColor = lambertTerm * vec4(1);
		fragColor += specular * vec4(1);
		//fragColor = R;
	}
	fragColor *= texture(diffuseTarget, uv)* texture(envTarget, uv);
	
	//fragColor = texture(finalAOTarget, uv);
{% endblock %}
