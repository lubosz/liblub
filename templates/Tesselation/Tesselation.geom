{% extends "base.glsl" %}

{% block header %}
uniform mat3 NormalMatrix;
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;
in vec3 tePosition[3];
in vec2 teUv[3];
in vec3 teColor1[3];
in vec3 teColor2[3];
in vec3 tePatchDistance[3];
out vec2 uv;
out vec3 color1;
out vec3 color2;
out vec3 gFacetNormal;
out vec3 gPatchDistance;
out vec3 gTriDistance;
{% endblock %}

{% block main %}
    vec3 A = tePosition[2] - tePosition[0];
    vec3 B = tePosition[1] - tePosition[0];
    gFacetNormal = NormalMatrix * normalize(cross(A, B));
    
    gPatchDistance = tePatchDistance[0];
    uv = teUv[0];
    color1 = teColor1[0];
    color2 = teColor2[0];
    
    gTriDistance = vec3(1, 0, 0);
    gl_Position = gl_in[0].gl_Position; EmitVertex();

    gPatchDistance = tePatchDistance[1];
    uv = teUv[1];
    color1 = teColor1[1];
    color2 = teColor2[1];
    gTriDistance = vec3(0, 1, 0);
    gl_Position = gl_in[1].gl_Position; EmitVertex();

    gPatchDistance = tePatchDistance[2];
    uv = teUv[2];
    color1 = teColor1[2];
    color2 = teColor2[2];
    gTriDistance = vec3(0, 0, 1);
    gl_Position = gl_in[2].gl_Position; EmitVertex();

    EndPrimitive();
{% endblock %}
