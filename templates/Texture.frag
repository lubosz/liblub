{% extends "base.frag" %}

{% block linkage %}
in vec2 uv;
{% endblock %}

{% block uniforms %}
uniform sampler2D color;
{% endblock %}

{% block main %}
	fragColor = texture(color, uv);
{% endblock %}
