#version 330 core

uniform sampler2D height;
uniform sampler2D normal;

in vec3 in_Vertex;
in vec3 in_Normal;
in vec2 in_Uv;

out vec2 uv;
out vec3 normalView;
out vec4 positionView;

uniform mat4 MVPMatrix;
uniform mat4 MVMatrix;
uniform mat3 NormalMatrix;
uniform vec2 uvoffset;

uniform float tiling;

void main(void) {
    //uv = (in_Uv + uvoffset)*tiling+vec2(tiling,tiling);
    uv = (in_Uv ) * tiling - vec2(.5, .5)*tiling + uvoffset;
    normalView = NormalMatrix * texture(normal, uv).rgb;
    //uv = (in_Uv + uvoffset);  
    float h = texture(height, uv).x;
    //p.y += h/tiling;
	vec3 position = in_Vertex + normalize(in_Normal) * h/tiling;
    vec4 p = vec4(position,1);

	positionView = MVMatrix * p;
    gl_Position = MVPMatrix * p;
}
