#version 410

in vec3 in_Vertex;
//out vec3 v_position;

//uniform mat4 MVPMatrix;

void main(void){
	//v_position = in_Vertex;
	//gl_Position = MVPMatrix * vec4(in_Vertex,1);
	gl_Position = vec4(in_Vertex,1);
}

