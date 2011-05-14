{% extends "Atmo/Atmo.vert" %}

{% block linkage %}
uniform mat3 NormalMatrix;
uniform mat4 MMatrix;
out vec3 color1;
out vec3 color2;
out vec3 normal;
{% endblock %}

{% block main %}
  {% include "Atmo/GroundMain.glsl" %}
  normal = in_Normal;
  gl_Position = MVPMatrix * vec4(in_Vertex,1);
{% endblock %}
