#version {{ version }}

{% if precision %}
precision highp float;
{% endif %}

{% block header %}{% endblock %}

void main() { 
{% block main %}{% endblock %}
}
