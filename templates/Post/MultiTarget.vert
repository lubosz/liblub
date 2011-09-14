{% extends "base.vert" %}

{% block linkage %}
in vec3 in_Normal;
in vec3 in_Tangent;
in vec2 in_Uv;

out vec4 positionView;
//out vec3 normalView;
out vec2 uv;
out vec4 positionWorld;
out vec4 normalWorld;
out vec4 tangentWorld;

uniform mat4 MMatrix;
uniform mat4 MVMatrix;
uniform mat3 NormalMatrix;
uniform vec4 camPositionWorld;

{% if isInstanced %}
uniform positions {
    vec4 cubePosition[{{positionElements}}];
};
{% endif %}

{% endblock %}

{% block main %}
	uv = in_Uv;
    vec4 position = vec4(in_Vertex,1);
	{% if isInstanced %}
        position += vec4(cubePosition[gl_InstanceID]);
    {% endif %}
	//normalView = normalize(NormalMatrix * in_Normal);
	positionView = MVMatrix * position;
	gl_Position = MVPMatrix * position;

	//reflection mapping
	positionWorld = MMatrix * position;
	normalWorld = normalize(MMatrix * vec4(in_Normal,0));
	tangentWorld = normalize(MMatrix * vec4(in_Tangent,0)); 
{% endblock %}