{% extends "base.frag" %}

{% block linkage %}
uniform float time; // Used for texture animation
in vec2 uv;
{% include "Fractal/classic3D.glsl" %}

{% endblock %}

{% block main %}
//3d
	//float noise =  pnoise(vec3(uv * 10,time*0.8),vec3(10));
	float noise =  cnoise(vec3(uv * 10,time*0.8));

//2d
	//float noise =  pnoise(uv*10,vec2(10,10));
	//float noise = cnoise(uv * 10);
	fragColor = noise * vec4(1);
{% endblock %}
