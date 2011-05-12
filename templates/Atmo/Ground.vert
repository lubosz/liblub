{% extends "Atmo/Atmo.vert" %}
{% block linkage %}
out vec3 color1;
out vec3 color2;
uniform mat4 MMatrix;
{% endblock %}

{% block main %}
	{% include "Atmo/GroundMain.glsl" %}
	gl_Position = MVPMatrix * vec4(in_Vertex,1);
{% endblock %}
