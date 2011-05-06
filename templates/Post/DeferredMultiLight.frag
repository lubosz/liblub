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
uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D tangentTexture;
uniform sampler2D normalTextureTexture;
uniform sampler2D envTexture;
uniform LightSourceBuffer {
	LightSource lightSources[5];
};
{% endblock %}


{% block main %}
	fragColor = vec4(0);
	for(int i = 0; i < 5 ; i++) {
		vec4 lightDirection = lightSources[i].position - texture(positionTexture, uv);
		vec4 L = normalize(lightDirection);	
		vec4 N = normalize(texture(normalTexture, uv));
		float lambertTerm = max( dot(N,L), 0.0);
		fragColor += lambertTerm * lightSources[i].diffuse;
	}
	fragColor *= texture(diffuseTexture, uv)* texture(envTexture, uv);
	
			

	//fragColor = texture(positionTexture, uv);
{% endblock %}
