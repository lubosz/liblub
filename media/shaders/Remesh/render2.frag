#version 330 core

precision highp float;

in  vec4 ex_Color;

out vec4 gl_FragColor;

void main(void) {
    //gl_FragColor = vec4(ex_Color,1.0);
	gl_FragColor = ex_Color;
}
