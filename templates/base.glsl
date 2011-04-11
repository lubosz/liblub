#version {{ version }}

{% if precision %}
precision highp float;
{% endif %}

{% block attributes %}
in vec3 in_Vertex;
in vec3 in_Normal;
in vec3 in_Tangent;
in vec2 in_Uv;
{% endblock %}

{% block out %}
out vec4 positionView;
out vec3 normalView;
#ifdef useNormalTexture
out vec3 lightVec;
out vec3 eyeVec;
#endif
out vec2 uv;
{% endblock %}

{% block uniforms %}
uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;
{%comment%}
{%endcomment%}
uniform vec4 lightPositionView;

#ifdef useHeightMap
uniform sampler2D heightMap;
uniform sampler2D vertexNormalMap;
#endif
{% endblock %}

{% block functions %}
{% endblock %}

void main() { 
{% block main %}
{% endblock %}
}
