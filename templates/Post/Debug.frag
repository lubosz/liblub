{% extends "base.frag" %}

{% block uniforms %}
uniform sampler2D {{ samplerName }};
{% endblock %}

{% block linkage %}
in vec2 uv;
{% endblock %}

{% block main %}
	fragColor = texture({{ samplerName }}, uv);
{% endblock %}