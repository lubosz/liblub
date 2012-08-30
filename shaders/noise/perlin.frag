{% extends "base.frag" %}

{% block linkage %}
uniform float time; // Used for texture animation
in vec2 uv;
in vec3 normal;

{% include "noise/classic3D.glsl" %}

float normalNoise(vec3 normal, float time) {
  return pnoise(vec3(normal.xy,time*0.8),vec3(0)) + pnoise(vec3(normal.yz*0.8,time),vec3(0));
}
{% endblock %}

{% block main %}
//  float n = cnoise(10*vec3(uv*5,time/20.0));
//  float nalt = cnoise(10*vec3(uv*2,time/20.0));
  float noiseor = normalNoise(normal*8,time/5.0);
  noiseor += normalNoise(normal*3,time/10.0);

  //fragColor = vec4(0.5 + 0.5 * vec3(n, n, n), 1.0);
  //vec4 red = vec4(0);
  //vec4 yellow = vec4(0);
  //red.r = (0.5 + 0.5 * vec3(n, n, n)).r;
  vec4 yellow = (0.5+noiseor) * vec4(1,1,0,0);
  vec4 red = vec4(1,0,0,0);
  fragColor = (yellow+red);
  
{% endblock %}
