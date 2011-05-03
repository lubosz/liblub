{% extends "base.vert" %}

{% block linkage %}
in vec2 in_Uv;
out vec2 uv;
{% endblock %}

{% block main %}
  uv = in_Uv;
  gl_Position = MVPMatrix * vec4(in_Vertex,1);
{% endblock %}
