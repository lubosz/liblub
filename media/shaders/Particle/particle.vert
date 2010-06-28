#version 150 core

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;

in vec3 in_Vertex;
in vec3 in_Normal;
in vec2 in_Uv;

out vec2 TexCoord;

void main()                                                            
{                                                                      
    //float pointSize = 500.0 * gl_Point.size;    
	float pointSize = 20.0;                       
    
	//vec4 vert = gl_Vertex;
    //vert.w = 1.0;	
	vec4 vert = vec4(in_Vertex,1.0);											
													
    //vec3 pos_eye = vec3 (gl_ModelViewMatrix * vert);
	vec3 pos_eye = vec3 (MVMatrix * vert);
                    
    gl_PointSize = max(1.0, pointSize / (1.0 - pos_eye.z)); 
	//gl_PointSize = 50; 
          
    //gl_TexCoord[0] = gl_MultiTexCoord0;   
    TexCoord        = in_Uv;
                             
                                       
    //gl_FrontColor = gl_Color;     

    //gl_Position = ftransform(); 
    gl_Position     = MVPMatrix * vec4(in_Vertex,1);                                     
}
