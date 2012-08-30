{% extends "base.glsl" %}

{% block header %}
in vec3 in_Vertex;
{% block linkage %}
{% endblock %}
uniform mat4 MVPMatrix;
{% block uniforms %}
{% endblock %}
{% endblock %}

{% block main %}
gl_Position = MVPMatrix * vec4(in_Vertex,1);
{% endblock %}
