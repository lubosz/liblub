#version 330 core

out vec4 gl_FragColor;
in vec4 viewPosition;

void main(void) {
    gl_FragColor = vec4(1,0,0,1);

    gl_FragDepth = gl_FragCoord.z; // [0..1] non-linear
    //gl_FragDepth = viewPosition.z; // [0..x] linear
}
