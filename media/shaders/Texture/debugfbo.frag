#version 330 core

//uniform sampler2DShadow shadowMap;
uniform sampler2D shadowMap;
//uniform sampler2DRectShadow shadowMap;

in vec3 position;
in vec2 uv;

out vec4 fragColor;

void main() {
	//fragColor = vec4(1)*textureProj(shadowMap, vec4(uv,1,1));
	//fragColor = vec4(1)*texture(shadowMap, vec3(uv,1),0.1);
	//fragColor = shadow2D(shadowMap, vec3(uv,1));
	//gl_FragDepth = position.x;
fragColor = texture(shadowMap, uv);
//fragColor = vec4(uv,0,1);
}

