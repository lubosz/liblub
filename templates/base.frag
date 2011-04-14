{% extends "base.glsl" %}

{% block header %}
  {% block linkage %}
  {% endblock %}
  {% block uniform %}
  {% endblock %}
out vec4 fragColor;
{% endblock %}

{% block main %}
fragColor = vec4(1,1,1,1);
{% endblock %}
