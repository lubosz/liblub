{% extends "base.frag" %}

{% block linkage %}
in vec2 uv;
in vec3 color;
{% endblock %}

{% block uniforms %}
uniform sampler2D glow;
{% endblock %}

{% block main %}
	fragColor = vec4(color,1) * texture(glow, uv);
	//fragColor = vec4(color,1);
{% endblock %}