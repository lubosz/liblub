{% extends "Atmo/Atmo.vert" %}

{% block linkage %}
uniform mat3 NormalMatrix;
uniform mat4 MMatrix;
out vec3 color1;
out vec3 color2;
out vec3 normal;
out vec3 position;
{% endblock %}

{% block main %}
  {% include "Atmo/GroundMain.glsl" %}
  normal = in_Normal;
  position = in_Vertex;
  gl_Position = MVPMatrix * vec4(in_Vertex,1);
{% endblock %}
