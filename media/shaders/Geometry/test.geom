#version 330
#extension GL_EXT_geometry_shader4 : enable

uniform mat4 MVPMatrix;

// Declare what type of incoming primitive our geometry shader is receiving
layout(triangles) in;

// Declare what type of primitives we are creating and the maximum amount of vertices we will output per use of the geometry shader.
// We will be outputting 6 vertices per use of this shader, creating 2 triangles.
layout(triangle_strip, max_vertices = 6) out;

// Inputs to geometry shaders are always as arrays in the quantity of the incoming primitive
// In our case this value is 3, since triangles have 3 vertices
in vec3 geom_Color[3];
in vec3 geom_Normal[3];
out vec3 ex_Color;


	void main(void)
	{
	
		//increment variable
		int i;

    ex_Color = vec3(0,0,0);




		/////////////////////////////////////////////////////////////
		//This example has two parts
		//	step a) draw the primitive pushed down the pipeline
		//		 there are gl_Vertices # of vertices
		//		 put the vertex value into gl_Position
		//		 use EmitVertex => 'create' a new vertex
		// 		use EndPrimitive to signal that you are done creating a primitive!
		//	step b) create a new piece of geometry (I.E. WHY WE ARE USING A GEOMETRY SHADER!)
		//		I just do the same loop, but swizzle the x and y values
		//	result => the line we want to draw, and the same line, but along the other axis




		//Pass-thru!
    // Create our original primitive
    for (i=0; i < gl_in.length(); i++)
    {
        gl_Position = MVPMatrix * gl_in[i].gl_Position;
        ex_Color = geom_Color[i];
        EmitVertex();
    }
    EndPrimitive();
    																						
		//New piece of geometry!  We just swizzle the x and y terms
    for (i=0; i < gl_in.length(); i++){
			gl_Position = MVPMatrix * (gl_in[i].gl_Position+vec4(geom_Normal[i],0)+vec4(4,0,0,0));
			//gl_Position.xy = gl_Position.yx;
			EmitVertex();
		}
		EndPrimitive();	
																			      /////////////////////////////////////////////////////////////

	}

