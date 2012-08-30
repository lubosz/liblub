#version 330 core

in vec3 in_Vertex;
in vec2 in_Uv;

out vec2 uv;

uniform mat4 MVPMatrix;

{% if useInstancing %}
uniform positions {
	vec4 cubePosition[{{positionElements}}];
};
{% endif %}

void main(void) {
    uv = in_Uv;
    {% if useInstancing %}
    vec4 move = vec4(cubePosition[gl_InstanceID]);
    vec4 movedVertex = vec4(in_Vertex,1)+move;
    {% else %}
    vec4 movedVertex = vec4(in_Vertex,1);
    {% endif %}
    gl_Position = MVPMatrix * movedVertex;
}
