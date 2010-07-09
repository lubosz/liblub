//VERTEX SHADER
#version 150
//uniform float Time;
out vec4 position; 
out vec3 normal; 
void main() 
{ 
	position = gl_Vertex; 
	normal = gl_Normal; 
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;
	
	gl_Position = gl_ModelViewMatrix * gl_Vertex;
	//gl_Position.xyz = gl_Position.xyz + (2.525 * ( abs(sin(0.0125*Time)) * gl_Normal));
	gl_Position = gl_ProjectionMatrix * gl_Position; 

	//gl_Position =  gl_ModelViewProjectionMatrix * gl_Vertex;
	//gl_Position.xyz = gl_Position.xyz*( abs(sin(0.0125*Time)));

}
