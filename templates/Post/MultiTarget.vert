{% extends "base.vert" %}

{% block linkage %}
in vec3 in_Normal;
in vec3 in_Tangent;
in vec2 in_Uv;

out vec4 positionView;
out vec3 normalView;
out vec3 tangentView;
out vec2 uv;

uniform mat4 MMatrix;
uniform mat4 MVMatrix;
uniform mat3 NormalMatrix;
{% endblock %}

{% block main %}
	uv = in_Uv;
    vec4 position = vec4(in_Vertex,1);
	//normalView = normalize(NormalMatrix * in_Normal);
	normalView = in_Normal;
	//normalmapping
	tangentView = normalize(NormalMatrix * in_Tangent); 
	positionView = MVPMatrix * position;
	gl_Position = MVPMatrix * position;
{% endblock %}