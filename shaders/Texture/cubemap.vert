{% extends "base.vert" %}

{% block linkage %}
in vec3 in_Normal;

uniform mat4 MVMatrix;
uniform mat3 NormalMatrix;

out vec3 reflectDir;
{% endblock %}

{% block main %}
	vec3 normal = normalize(NormalMatrix * in_Normal);
	vec3 eyeDirecton = vec3(MVMatrix * vec4(in_Vertex,1));
	reflectDir = reflect(-eyeDirecton, normal);
	gl_Position = MVPMatrix * vec4(in_Vertex,1);
{% endblock %}

