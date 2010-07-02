#version 330 core

//in vec4 ex_Color;
in vec2 tex_Coord;
out vec4 gl_FragColor;

uniform sampler2D terrain_texture;

void main(void) {
    gl_FragColor = texture(terrain_texture, tex_Coord);
	//gl_FragColor = ex_Color;
//gl_FragColor = vec4(1,1,1,1);
}
