{% extends "base.vert" %}

{% block linkage %}
in vec2 in_Uv;
out vec2 vUv;
out vec3 vPosition;
{% endblock %}

{% block main %}
	vUv = in_Uv;
    vPosition = in_Vertex.xyz;
{% endblock %}
