{% extends "base.frag" %}

{% block linkage %}
in vec2 uv;
in vec3 color1;
in vec3 color2;
{% endblock %}

{% block uniforms %}
uniform sampler2D planet;
{% endblock %}

{% block main %}
	fragColor = vec4(color1,1) + texture(planet, uv) * vec4(color2,1);
{% endblock %}
