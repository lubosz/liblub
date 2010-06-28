#version 150 core

uniform sampler2D splatTexture;

in vec2 TexCoord;

out vec4 FragColor;
                                      
void main()                                                            
{                                                                      
    //vec4 color = (0.6 + 0.4 * gl_Color) * texture2D(splatTexture, gl_TexCoord[0].st);
	//vec4 color = (0.6 + 0.4 * gl_Color) * texture2D(splatTexture, TexCoord);
	vec4 color = (0.6 + 0.4 * vec4(1,1,1,1)) * texture2D(splatTexture, TexCoord);
    FragColor = color * mix(vec4(0.0, 0.2, 0.2, color.w), vec4(0.2, 0.7, 0.7, color.w), color.w);
}  
