{% extends "base.frag" %}

{% block linkage %}
in vec4 viewPosition;
{% endblock %}

{% block main %}
    fragColor = vec4(1,0,0,1);

    gl_FragDepth = gl_FragCoord.z; // [0..1] non-linear
    //gl_FragDepth = viewPosition.z; // [0..x] linear
{% endblock %}
