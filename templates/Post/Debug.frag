{% extends "base.frag" %}

{% block uniforms %}
    uniform sampler2D {{ samplerName }};
{% endblock %}

{% block linkage %}
in vec2 uv;
{% endblock %}

{% block main %}
{% if isDepthTexture %}
    float depth = texture({{ samplerName }}, uv).r;
    fragColor = vec4(depth);
{% else %}
    fragColor = texture({{ samplerName }}, uv);
{% endif %}
{% endblock %}
