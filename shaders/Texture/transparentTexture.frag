{% extends "Texture/texture.frag" %}

{% block main %}
	fragColor = texture(targetTexture, uv);
	fragColor.a = 0.5;
{% endblock %}
