#version 330 core

uniform vec3 brickColor, mortarColor;
uniform vec2 brickSize, brickPct;

in vec2 positionModel;
in float lightIntensity;
out vec4 fragColor;

void main(){
	vec2 position = positionModel / brickSize;
	if (fract(position.y * 0.5) > 0.5)
		position.x += 0.5;
	position = fract(position);
	vec2 useBrick = step(position, brickPct);

	fragColor = vec4(mix(mortarColor, brickColor, useBrick.x * useBrick.y),1);
	fragColor *= lightIntensity;
}

