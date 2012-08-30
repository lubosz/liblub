{% extends "base.frag" %}

{% block linkage %}
uniform sampler2D diffuse;
in vec2 uv;
{% endblock %}

{% block main %}
	fragColor    = texture(diffuse, uv);
{% endblock %}
