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
{% endblock %}

{% block uniforms %}
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
{% endblock %}


{% block main %}
	fragColor = positionView;
	normalColor = vec4(normalView,1);
	tangentColor =  vec4(tangentView,1);
	diffuseColor = texture(diffuseTexture, uv);
	normalTextureColor = texture(normalTexture, uv);
{% endblock %}
