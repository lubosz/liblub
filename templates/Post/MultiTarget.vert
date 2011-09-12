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

{% if isInstanced %}
uniform positions {
    vec4 cubePosition[{{positionElements}}];
};
{% endif %}

{% endblock %}

{% block main %}
	uv = in_Uv;
	{% if isInstanced %}
        vec4 move = vec4(cubePosition[gl_InstanceID]);
        vec4 position = vec4(in_Vertex,1)+move;
    {% else %}
        vec4 position = vec4(in_Vertex,1);
    {% endif %}
	//normalView = normalize(NormalMatrix * in_Normal);
	normalView = normalize(NormalMatrix * in_Normal);
	//normalmapping
	tangentView = normalize(NormalMatrix * in_Tangent); 
	positionView = MVMatrix * position;
	gl_Position = MVPMatrix * position;
{% endblock %}