{% extends "base.frag" %}

{% block linkage %}
uniform sampler2D envMap;

in vec3 reflectDir;
{% endblock %}

{% block main %}
	vec2 uv;
	vec3 r = -reflectDir;
	float m = 2.0 * sqrt( r.x*r.x + r.y*r.y + (r.z+1.0)*(r.z+1.0) );
	uv.x = r.x/m + 0.5;
	uv.y = r.y/m + 0.5;
	
	fragColor = texture(envMap, uv);
{% endblock %}

