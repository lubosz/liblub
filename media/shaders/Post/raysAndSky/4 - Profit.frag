#version 330 core

uniform sampler2D DepthBuffer;
uniform sampler2D Pass2;
uniform sampler2D Pass3;

in vec2 TexCoord;

out vec4 FragColor;

float blurSize=0.0015;

float getDepth(sampler2D texture,vec2 TexCoord){
	return dot(texture2D(texture,TexCoord), vec4(0.000000059604644775390625,0.0000152587890625,0.00390625,1.0))*10000.0;
}

void main(void){
	vec4 color=vec4(0.0,0.0,0.0,0.0);
	color += texture2D(Pass3, vec2(TexCoord.x, TexCoord.y - 4.0*blurSize)) * 0.05;
	color += texture2D(Pass3, vec2(TexCoord.x, TexCoord.y - 3.0*blurSize)) * 0.09;
	color += texture2D(Pass3, vec2(TexCoord.x, TexCoord.y - 2.0*blurSize)) * 0.12;
	color += texture2D(Pass3, vec2(TexCoord.x, TexCoord.y - blurSize)) * 0.15;
	color += texture2D(Pass3, vec2(TexCoord.x, TexCoord.y)) * 0.16;
	color += texture2D(Pass3, vec2(TexCoord.x, TexCoord.y + blurSize)) * 0.15;
	color += texture2D(Pass3, vec2(TexCoord.x, TexCoord.y + 2.0*blurSize)) * 0.12;
	color += texture2D(Pass3, vec2(TexCoord.x, TexCoord.y + 3.0*blurSize)) * 0.09;
	color += texture2D(Pass3, vec2(TexCoord.x, TexCoord.y + 4.0*blurSize)) * 0.05;
	float blur=getDepth(DepthBuffer,TexCoord)-getDepth(DepthBuffer,vec2(0.5,0.35));
	blur=clamp(pow(blur/5.0,2.0),0.0,1.0);
	color = texture2D(Pass2, TexCoord)*(1.0-blur)+color*blur;
	FragColor = vec4(color.rgb,1.0);
}
