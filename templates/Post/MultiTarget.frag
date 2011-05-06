{% extends "base.frag" %}

{% block linkage %}
in vec4 positionView;
in vec3 normalView;
in vec3 tangentView;
in vec2 uv;
out vec4 normalColor;
out vec4 tangentColor;
out vec4 diffuseColor;
out vec4 normalTextureColor;
out vec4 envColor;
{% endblock %}

{% block uniforms %}
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform samplerCube envMap;
{% endblock %}


{% block main %}
	fragColor = positionView;
	normalColor = vec4(normalView,1);
	tangentColor =  vec4(tangentView,1);
	diffuseColor = texture(diffuseTexture, uv*5);
	normalTextureColor = texture(normalTexture, uv);
	vec3 reflectDir = reflect(-positionView.xyz, normalView);
	envColor = texture(envMap, reflectDir);
{% endblock %}
