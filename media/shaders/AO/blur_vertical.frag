#version 330 core

uniform sampler2D blurH;

in vec2 uv;
out vec4 fragColor;

void main() {
	vec4 sum = vec4(0);
	   
	const float blurSize = 1.0/1200.0;
 
	// blur in y (vertical)
	// take nine samples, with the distance blurSize between them
	sum += texture(blurH, vec2(uv.x, uv.y - 4.0*blurSize)) * 0.05;
	sum += texture(blurH, vec2(uv.x, uv.y - 3.0*blurSize)) * 0.09;
	sum += texture(blurH, vec2(uv.x, uv.y - 2.0*blurSize)) * 0.12;
	sum += texture(blurH, vec2(uv.x, uv.y - blurSize)) * 0.15;
	sum += texture(blurH, vec2(uv.x, uv.y)) * 0.16;
	sum += texture(blurH, vec2(uv.x, uv.y + blurSize)) * 0.15;
	sum += texture(blurH, vec2(uv.x, uv.y + 2.0*blurSize)) * 0.12;
	sum += texture(blurH, vec2(uv.x, uv.y + 3.0*blurSize)) * 0.09;
	sum += texture(blurH, vec2(uv.x, uv.y + 4.0*blurSize)) * 0.05;
 
	fragColor = sum;
}