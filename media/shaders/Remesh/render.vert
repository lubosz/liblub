#version 330 core

in vec3 in_Vertex;
//in vec3 in_Color;
in vec3 in_Normal;
in vec2 in_Uv;

//out vec4 front_color;
out vec2 TexCoord;

uniform sampler2D displacement_texture;
//uniform sampler2D terrain_texture;

//uniform float scale;

//uniform mat4 MVPMatrix;

void main(void)
{
	float scale = 1.0;
	TexCoord = in_Uv;
	//vec4 dispColor = texture2D(displacement_texture, in_Color.xy);
	vec4 dispColor = texture2D(displacement_texture, in_Uv);

	//gl_Position = vec4(in_Vertex,1.0) + vec4(in_Normal*dispColor.x*scale,1.0);
    	//gl_Position     = MVPMatrix * vec4(in_Vertex,1);
	gl_Position     = vec4(in_Vertex,1);
	//gl_FrontColor= texture2D(terrain_texture, gl_Color.xy);
	
//	front_color = texture2D(terrain_texture, in_Uv);
}
