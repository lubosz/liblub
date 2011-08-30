{% extends "base.frag" %}

{% block uniforms %}
uniform sampler2D {{ source1 }};
uniform sampler2D {{ source2 }};
float ratio = 1.0 / 2.0;
{% endblock %}

{% block linkage %}
in vec2 uv;
{% endblock %}

{% block main %}
	vec4 value1 = texture({{ source1 }}, uv);
	vec4 value2 = texture({{ source2 }}, uv);
	//fragColor = ratio * texture({{ source1 }}, uv) + (1-ratio) * texture({{ source2 }}, uv);
	fragColor =  (value1* value2) + value2*0.2; 
{% endblock %}