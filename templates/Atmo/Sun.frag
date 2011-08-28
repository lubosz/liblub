{% extends "base.frag" %}

{% block linkage %}
uniform float time; // Used for texture animation
uniform float cameraHeight2;
uniform float innerRadius2;
uniform float cameraHeight;
uniform float innerRadius;
uniform sampler2D diffuse;
uniform sampler2D planet;
in vec2 uv;
in vec3 normal;
in vec3 color1;
in vec3 color2;
in vec3 position;

{% include "noise/simplex3D.glsl" %}

/*
float normalNoise(vec3 normal, float time) {
  return pnoise(vec3(normal.xy,time*0.8),vec3(0)) + pnoise(vec3(normal.yz*0.8,time),vec3(0));
}
*/
vec4 getColorForAltitude(float altitude){
	if (altitude < 4){
		return vec4(1,0,0,1);
	} else if (altitude < 16) {
		return vec4(1,1,0,1);
	} else if (altitude < 36) {
		return vec4(1,0,1,1);
	} else if (altitude < 64) {
		return vec4(0,1,1,1);
	} else if (altitude < 100) {
		return vec4(0,1,0,1);
	} else {
		return vec4(0,0,1,1);
	}
}

float multiSampleNoise(vec3 uvw, float time) {
  	float n = snoise(uvw - vec3(0.0, 0.0, time));
	n += 0.5 * snoise(uvw * 2.0 - vec3(0.0, 0.0, time*1.4));
	n += 0.25 * snoise(uvw * 4.0 - vec3(0.0, 0.0, time*2.0));
	n += 0.125 * snoise(uvw * 8.0 - vec3(0.0, 0.0, time*2.8));
	n += 0.0625 * snoise(uvw * 16.0 - vec3(0.0, 0.0, time*4.0));
	n += 0.03125 * snoise(uvw * 32.0 - vec3(0.0, 0.0, time*5.6));
	n = n * 0.7;
	return n;
}
{% endblock %}

{% block main %}

	float timeControl = time/10.0;
 	vec3 uvw = position + 0.1*
	vec3(
		snoise(position + vec3(0.0, 0.0, timeControl)),
    	snoise(position + vec3(43.0, 17.0, timeControl)),
 		snoise(position + vec3(-17.0, -43.0, timeControl))
 	);
  //vec3 uvw = position;	
	
	float n = multiSampleNoise(uvw, timeControl);
	fragColor = vec4(vec3(1.0, 0.5, 0.0) + vec3(n, n, n), 1.0);
//	fragColor = vec4(n,n,n,1);
  fragColor = vec4(color1,1) + fragColor * vec4(color2,1);
{% endblock %}
