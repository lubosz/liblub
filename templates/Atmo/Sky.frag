{% extends "base.frag" %}

{% block linkage %}
in vec3 v3Direction;
in vec3 color1;
in vec3 color2;
{% endblock %}

{% block uniforms %}
uniform vec3 v3LightPos;
uniform float g;
uniform float g2;
{% endblock %}

{% block main %}
	float fCos = dot(v3LightPos, v3Direction) / length(v3Direction);
	float fMiePhase = 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos*fCos) / pow(1.0 + g2 - 2.0*g*fCos, 1.5);
	fragColor = vec4(color1 + fMiePhase * color2,1);
	fragColor.a = fragColor.b;
{% endblock %}
