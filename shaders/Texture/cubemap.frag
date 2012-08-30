{% extends "base.frag" %}

{% block linkage %}
uniform samplerCube EnvMap;

in vec3 reflectDir;
{% endblock %}

{% block main %}
	fragColor = texture(EnvMap, reflectDir);
{% endblock %}

