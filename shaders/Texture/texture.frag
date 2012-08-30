{% extends "base.frag" %}

{% block linkage %}
in vec2 uv;
{% endblock %}

{% block uniforms %}
uniform sampler2D targetTexture;
{% endblock %}

{% block main %}
	fragColor = texture(targetTexture, uv);
{% endblock %}
