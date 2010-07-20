#version 120

uniform sampler2D terrain_texture;
uniform sampler2D displacement_texture;

uniform float scale;

void main(void)
{
	vec4 dispColor = texture2D(displacement_texture, gl_Color.xy);
	gl_Position = gl_Vertex + vec4(gl_Normal*dispColor.x*scale,1.0);
	gl_FrontColor= texture2D(terrain_texture, gl_Color.xy);
}
