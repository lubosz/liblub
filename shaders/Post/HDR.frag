{% extends "base.frag" %}

{% block linkage %}
in vec2 uv;
{% endblock %}

{% block uniforms %}
uniform sampler2D targetTexture;
//uniform sampler2DRect targetTexture;
uniform float exposure;
{% endblock %}


{% block main %}
	vec4 color = texture(targetTexture, uv);
	//vec4 color = textureRect(targetTexture, uv * 1024.0);
	fragColor = 1.0 - exp(color * -exposure);
{% endblock %}
