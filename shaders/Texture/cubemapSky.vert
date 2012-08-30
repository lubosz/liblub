{% extends "base.vert" %}

{% block linkage %}
out vec3 reflectDir;
{% endblock %}

{% block main %}
    reflectDir = -in_Vertex;
    gl_Position = MVPMatrix * vec4(in_Vertex, 1);
{% endblock %}

