#version 330 core

precision highp float;

uniform mat4 MVPMatrix;

// Declare what type of incoming primitive our geometry shader is receiving
layout(triangles) in;

// Declare what type of primitives we are creating and the maximum amount of vertices we will output per use of the geometry shader.
// We will be outputting 6 vertices per use of this shader, creating 2 triangles.
//layout(triangles, max_vertices = 400) out;

//in vec4 front_color[3];
in vec2 TexCoord[3];
out vec2 tex_Coord;
//out vec4 ex_Color;

/*
out vec4 tf_data;
out vec3 tf_position;
out vec3 tf_normal;
*/
void main(void) {
	//tf_normal = vec3(0.0, 0.0, 1.0);
	//for (int i=0; i<3; i++) {
	for (i=0; i < gl_in.length(); i++)
		gl_Position = MVPMatrix * gl_in[i].gl_Position;
		//gl_Position = gl_in[i].gl_Position.xyz;
		//ex_Color = front_Color[i];
		tex_Coord = TexCoord[i];
		EmitVertex();

		//tf_data = gl_FrontColorIn[i];
		//tf_position=gl_PositionIn[i].xyz;
		//EmitVertex();
	}

	EndPrimitive();
}

