{% extends "base.frag" %}

{% block linkage %}
in vec2 uv;
{% endblock %}

{% block uniforms %}
uniform sampler2D targetTexture;
//uniform sampler2DRect s2Test;	// RECTANGLE textures supported in GLSL?
uniform float fExposure;
{% endblock %}


{% block main %}
	vec4 color = texture(targetTexture, uv);
	//vec4 f4Color = texture2DRect(s2Test, gl_TexCoord[0].st * 1024.0);
	//fragColor = 1.0 - exp(color * -fExposure);
	fragColor = color;
{% endblock %}
