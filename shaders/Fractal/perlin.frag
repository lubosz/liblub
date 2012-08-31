{% extends "base.frag" %}

{% block linkage %}
uniform float time; // Used for texture animation
in vec2 uv;
{% include "Fractal/classic3D.glsl" %}

{% endblock %}

{% block main %}
	float noise =  pnoise(vec3(uv * 10,time*0.8),vec3(0));
	fragColor = noise * vec4(1);
{% endblock %}
