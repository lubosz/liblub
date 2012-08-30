{% extends "base.frag" %}

{% block linkage %}
in vec2 uv;
in vec3 color1;
in vec3 color2;
in vec3 gFacetNormal;
in vec3 gTriDistance;
in vec3 gPatchDistance;
in float gPrimitive;
{% endblock %}

{% block uniforms %}
uniform vec3 LightPosition;
uniform vec3 DiffuseMaterial;
uniform vec3 AmbientMaterial;
uniform sampler2D diffuse;
//uniform sampler2D planet;

float amplify(float d, float scale, float offset)
{
    d = scale * d + offset;
    d = clamp(d, 0, 1);
    d = 1 - exp2(-2*d*d);
    return d;
}

{% endblock %}

{% block main %}
    vec3 N = normalize(gFacetNormal);
    vec3 L = LightPosition;
    float df = abs(dot(N, L));
/*
    vec3 color = AmbientMaterial + df * DiffuseMaterial;

    float d1 = min(min(gTriDistance.x, gTriDistance.y), gTriDistance.z);
    float d2 = min(min(gPatchDistance.x, gPatchDistance.y), gPatchDistance.z);
    color = amplify(d1, 40, -0.5) * amplify(d2, 60, -0.5) * color;

    fragColor = vec4(color, 1.0);
*/
    vec4 color = texture(diffuse,uv)-texture(diffuse,uv*2.5)+texture(diffuse,uv*3.1)*df;
    fragColor = vec4(color1,1) + color * vec4(color2,1);
{% endblock %}

