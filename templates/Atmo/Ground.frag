{% extends "base.frag" %}

{% block linkage %}
in vec2 uv;
in vec3 color1;
in vec3 color2;
{% endblock %}

{% block uniforms %}
uniform float cameraHeight;
uniform float innerRadius;
uniform sampler2D diffuse;
uniform sampler2D planet;
{% endblock %}

{% block main %}	
	fragColor = texture(diffuse, uv*6);
	fragColor += texture(diffuse, uv*12);
	fragColor += texture(diffuse, uv*25);
	fragColor += texture(diffuse, uv*50);
	fragColor += texture(diffuse, uv*100);
	fragColor += texture(diffuse, uv*200);
	fragColor += texture(diffuse, uv*400);
	fragColor += texture(diffuse, uv*800);
	fragColor /= 8.0f;
	fragColor *=  texture(planet,uv);
	/*
	float wholePlanetFactor = clamp((cameraHeight - innerRadius - 1)/10.0,0.0,1.0);
	fragColor = mix(fragColor, texture(planet,uv), wholePlanetFactor);
	fragColor = vec4(color1,1) + texture(planet, uv) * vec4(color2,1);
	*/
{% endblock %}
