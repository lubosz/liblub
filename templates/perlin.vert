{% extends "base.vert" %}

{% block linkage %}
in vec3 in_Normal;
in vec2 in_Uv;
out vec2 uv;
out vec3 normal;
uniform mat3 NormalMatrix;
{% endblock %}

{% block main %}
  uv = in_Uv;
  normal = in_Normal;
  gl_Position = MVPMatrix * vec4(in_Vertex,1);
{% endblock %}
