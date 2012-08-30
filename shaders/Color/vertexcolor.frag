{% extends "base.frag" %}

{% block linkage %}
in vec3 color;
{% endblock %}

{% block main %}
	fragColor = vec4(color, 1);
{% endblock %}
