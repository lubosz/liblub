{% extends "base.frag" %}

{% block linkage %}
in vec3 v3Direction;
in vec3 color1;
in vec3 color2;
{% endblock %}

{% block uniforms %}
uniform vec3 lightPosition;
uniform float g;
uniform float g2;
{% endblock %}

{% block main %}
	float cos = dot(lightPosition, v3Direction) / length(v3Direction);
	float miePhase = 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + cos*cos) / pow(1.0 + g2 - 2.0*g*cos, 1.5);
	fragColor = vec4(color1 + miePhase * color2,1);
	fragColor.a = fragColor.b;
{% endblock %}
