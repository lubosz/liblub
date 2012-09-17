{% extends "base.frag" %}

{% block linkage %}
in  vec3 geom_Color;
{% endblock %}

{% block main %}
    fragColor = vec4(geom_Color,1.0);
{% endblock %}
