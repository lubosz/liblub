{% extends "base.vert" %}

{% block linkage %}

  {% if uv %}
    in vec2 in_Uv;
    out vec2 uv;
  {% endif %}

  {% if normalmap %}
    in vec3 in_Tangent;
    in vec3 in_Bitangent;
    out vec3 lightVec;
    out vec3 eyeVec;
  {% endif %}
  
  {% if viewposition %}
    uniform mat4 MVMatrix;
    out vec4 positionView;
  {% endif %}
    
  {% if normal %}
    {% include "Shading/normal.vert" %}
  {% endif %}
{% endblock %}

{% block uniforms %}
  uniform mat4 MVPMatrix;
{% endblock %}

{% block main %}
  vec4 position = vec4(in_Vertex,1);

  {% if uv %}
  	uv = in_Uv;
  {% endif %}

  {% if normal %}
	  normalView = normalize(NormalMatrix * in_Normal);
  {% endif %}

  {% if viewposition %}
  	positionView = MVMatrix * position;
  {% endif %}

  {% if normalmap %}
    tangentSpace(positionView);
  {% endif %}
	gl_Position = MVPMatrix * position;
{% endblock %}
