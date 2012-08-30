#version {{ version }}

{% if precision %}
precision highp float;
{% endif %}

{% block header %}{% endblock %}

{% block functions %}{% endblock %}

void main() { 
{% block main %}{% endblock %}
}
