#version 330 core

//uniform sampler2D Pass2;
uniform sampler2D FBOTex;
uniform vec3 LightPosition;

in vec2 TexCoord;

out vec4 FragColor;

float blurSize=0.0015;
	
void main(void){
	vec4 color=vec4(0.0,0.0,0.0,0.0);
	color += texture2D(FBOTex, vec2(TexCoord.x - 4.0*blurSize, TexCoord.y)) * 0.05;
	color += texture2D(FBOTex, vec2(TexCoord.x - 3.0*blurSize, TexCoord.y)) * 0.09;
	color += texture2D(FBOTex, vec2(TexCoord.x - 2.0*blurSize, TexCoord.y)) * 0.12;
	color += texture2D(FBOTex, vec2(TexCoord.x - blurSize, TexCoord.y)) * 0.15;
	color += texture2D(FBOTex, vec2(TexCoord.x, TexCoord.y)) * 0.16;
	color += texture2D(FBOTex, vec2(TexCoord.x + blurSize, TexCoord.y)) * 0.15;
	color += texture2D(FBOTex, vec2(TexCoord.x + 2.0*blurSize, TexCoord.y)) * 0.12;
	color += texture2D(FBOTex, vec2(TexCoord.x + 3.0*blurSize, TexCoord.y)) * 0.09;
	color += texture2D(FBOTex, vec2(TexCoord.x + 4.0*blurSize, TexCoord.y)) * 0.05;
	FragColor = vec4(color.rgb,1.0);
}

