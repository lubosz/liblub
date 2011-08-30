#version 330 core

in vec4 viewPosition;
in vec3 viewNormal;

out vec4 normalTarget;
out vec4 depthTarget;

void main(void) {
    normal = vec4(viewNormal, 1);
    gl_FragDepth = viewPosition.z/30;
}
