#version 330 core

//uniform sampler2D Pass2;
uniform sampler2D RenderTexture;

in vec2 uv;

out vec4 fragColor;

float blurSize=0.0015;
	
void main(void){
	vec4 color=vec4(0.0,0.0,0.0,0.0);
	color += texture2D(RenderTexture, vec2(uv.x - 4.0*blurSize, uv.y)) * 0.05;
	color += texture2D(RenderTexture, vec2(uv.x - 3.0*blurSize, uv.y)) * 0.09;
	color += texture2D(RenderTexture, vec2(uv.x - 2.0*blurSize, uv.y)) * 0.12;
	color += texture2D(RenderTexture, vec2(uv.x - blurSize, uv.y)) * 0.15;
	color += texture2D(RenderTexture, vec2(uv.x, uv.y)) * 0.16;
	color += texture2D(RenderTexture, vec2(uv.x + blurSize, uv.y)) * 0.15;
	color += texture2D(RenderTexture, vec2(uv.x + 2.0*blurSize, uv.y)) * 0.12;
	color += texture2D(RenderTexture, vec2(uv.x + 3.0*blurSize, uv.y)) * 0.09;
	color += texture2D(RenderTexture, vec2(uv.x + 4.0*blurSize, uv.y)) * 0.05;
	fragColor = vec4(color.rgb,1.0);
	
}

