{% extends "base.frag" %}

{% block linkage %}
in vec3 v3Direction;
in vec3 color1;
in vec3 color2;
in vec3 debugColor;
{% endblock %}

{% block uniforms %}
uniform vec3 lightPosition;
uniform float g;
uniform float g2;
uniform bool useMie;
uniform bool useRayleigh;

// Calculates the Mie phase function
float getMiePhase(float cos, float cos2, float g, float g2) {
	return 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + cos2) / pow(1.0 + g2 - 2.0*g*cos, 1.5);
}

// Calculates the Rayleigh phase function
float getRayleighPhase(float cos2) {
	//return 1.0;
	return 0.75 + 0.75*cos2;
}

{% endblock %}

{% block main %}
	float miePhase = 0;
	float rayleighPhase = 0;

	float cos = dot(lightPosition, v3Direction) / length(v3Direction);
	//float miePhase = 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + cos*cos) / pow(1.0 + g2 - 2.0*g*cos, 1.5);
	float cos2 = cos * cos;
	if (useMie) {
		miePhase = getMiePhase(cos, cos2, g, g2);
	} else {
		miePhase = 1.0;
	}
	if (useRayleigh) {
		rayleighPhase = getRayleighPhase(cos2);
	} else {
		rayleighPhase = 1.0;
	}
	fragColor = vec4(rayleighPhase*color1 + miePhase * color2,1);
	//fragColor = vec4(color1+ color2,1);
	//fragColor.a = fragColor.b;
	//fragColor = vec4(debugColor,1);
	//fragColor = vec4(1)*miePhase;
{% endblock %}
