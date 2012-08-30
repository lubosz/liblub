{% extends "base.vert" %}

{% block linkage %}
in vec3 in_Color;

out vec3 color;
{% endblock %}

{% block main %}
  color = in_Color;
	gl_Position = MVPMatrix * vec4(in_Vertex,1);
{% endblock %}
