#version 330 core

//precision highp float;

in vec3 in_Vertex;
//in vec3 in_Normal;
//in vec3 in_Tangent;
//in vec3 in_Bitangent;
in vec2 in_Uv;

out vec2 uv;
/*
out vec3 viewNormal;
out float viewDistance;
uniform mat4 MVMatrix;
uniform mat3 NormalMatrix;
*/
uniform mat4 MVPMatrix;

{% if useInstancing %}
uniform positions {
	vec4 cubePosition[{{positionElements}}];
};
{% endif %}

void main(void) {
    uv = in_Uv;
    //viewNormal = NormalMatrix * in_Normal;
    {% if useInstancing %}
    vec4 move = vec4(cubePosition[gl_InstanceID]);
    vec4 movedVertex = vec4(in_Vertex,1)+move;
    {% else %}
    vec4 movedVertex = vec4(in_Vertex,1);
    {% endif %}
    //viewDistance = length(MVMatrix *movedVertex);
    gl_Position = MVPMatrix * movedVertex;
}
