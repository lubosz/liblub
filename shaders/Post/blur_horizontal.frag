#version {{ version }}

uniform sampler2D merge;

in vec2 uv;

out vec4 fragColor;

void main() {
	vec4 sum = vec4(0);
	   
	const float blurSize = 1.0/1920.0;
 
	// blur in y (vertical)
	// take nine samples, with the distance blurSize between them
	sum += texture(merge, vec2(uv.x - 4.0*blurSize, uv.y)) * 0.05;
	sum += texture(merge, vec2(uv.x - 3.0*blurSize, uv.y)) * 0.09;
	sum += texture(merge, vec2(uv.x - 2.0*blurSize, uv.y)) * 0.12;
	sum += texture(merge, vec2(uv.x - blurSize, uv.y)) * 0.15;
	sum += texture(merge, vec2(uv.x, uv.y)) * 0.16;
	sum += texture(merge, vec2(uv.x + blurSize, uv.y)) * 0.15;
	sum += texture(merge, vec2(uv.x + 2.0*blurSize, uv.y)) * 0.12;
	sum += texture(merge, vec2(uv.x + 3.0*blurSize, uv.y)) * 0.09;
	sum += texture(merge, vec2(uv.x + 4.0*blurSize, uv.y)) * 0.05;

	fragColor = sum;
}