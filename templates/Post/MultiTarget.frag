{% extends "base.frag" %}

{% block linkage %}
in vec4 positionView;
in vec3 normalView;
in vec3 tangentView;
in vec2 uv;
out vec4 positionTarget;
out vec4 normalTarget;
out vec4 diffuseTarget;
out vec4 tangentTarget;
out vec4 normalMapTarget;
out vec4 envTarget;
//out float depthTarget;
{% endblock %}

{% block uniforms %}
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform samplerCube envMap;
{% endblock %}


{% block main %}
	positionTarget = positionView;
	normalTarget = vec4(normalView,1);
	tangentTarget =  vec4(tangentView,1);
	diffuseTarget = texture(diffuseTexture, uv*5);
	normalMapTarget = texture(normalTexture, uv);
	vec3 reflectDir = reflect(-positionView.xyz, normalView);
	envTarget = texture(envMap, reflectDir);
	gl_FragDepth = positionView.z/20;
	//depthTarget = positionView.z/20;
{% endblock %}
