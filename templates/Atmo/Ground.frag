{% extends "base.frag" %}

{% block linkage %}
in vec2 uv;
in vec3 color1;
in vec3 color2;
{% endblock %}
{% block uniform %}
uniform sampler2D planet;
{% endblock %}

{% block main %}
	//gl_FragColor = gl_Color + 0.25 * gl_SecondaryColor;
	//gl_FragColor = gl_Color + texture2D(s2Tex1, gl_TexCoord[0].st) * texture2D(s2Tex2, gl_TexCoord[1].st) * gl_SecondaryColor;

	//fragColor = vec4(color1 + 0.25 * color2,1);
	//fragColor = texture(planet, uv);
	//fragColor = vec4(1);

	fragColor = vec4(color1,1) + texture(planet, uv) * vec4(color2,1);
{% endblock %}
