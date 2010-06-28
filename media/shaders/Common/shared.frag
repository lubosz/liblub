#version 150 core

//  starting texture coordinate
in vec2 texCoord;
//  position and light in eye space
in vec3 eyeSpaceVert;
//out vec3 eyeSpaceLight;
//  the matrix needed to convert to eye space
//varying mat3 eyeTransform;
//*
in vec3 eyeSpaceTangent;
in vec3 eyeSpaceBinormal;
in vec3 eyeSpaceNormal;

out vec4 gl_FragColor;

void main(void) {
    gl_FragColor = vec4(texCoord,1.0,1.0);
}
