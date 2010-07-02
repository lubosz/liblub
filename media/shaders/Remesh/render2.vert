#version 330 core

precision highp float;

in vec3 in_Vertex;
//in vec3 in_Color;
in vec3 in_Normal;
in vec2 in_Uv;

//out vec3 geom_Color;

uniform sampler2D displacement_texture;
uniform sampler2D terrain_texture;

out vec4 front_color;

void main(void) {

	float scale = 1.0;
	vec4 dispColor = texture2D(displacement_texture, in_Uv);
	gl_Position = vec4(in_Vertex,1.0) + vec4(in_Normal*dispColor.x*scale,1.0);

	front_color = texture2D(terrain_texture, in_Uv);
	//gl_Position = vec4(in_Vertex,1);
}
