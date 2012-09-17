{% extends "base.vert" %}

{% block linkage %}
out vec4 viewPosition;
{% endblock %}

{% block main %}
	viewPosition = MVPMatrix * vec4(in_Vertex,1);
	gl_Position = MVPMatrix * vec4(in_Vertex,1);
{% endblock %}
