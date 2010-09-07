#version 150 core

// maximum size supported by this shader
const int MaxKernelSize = 25;

// array of offsets for accessing the base image
uniform vec2 Offset[MaxKernelSize];
// size of kernel (width * height) for this execution
uniform int KernelSize;
// final scaling value
uniform vec4 ScaleFactor;
// image to be convolved
uniform sampler2D BaseImage;

// texture coordinate output from vertex shader
in vec2 TexCoord;

// fragment shader output
out vec4 FragColor;

void main()
{
    int i;
    vec4 sum = vec4(0.0);
    for (i = 0; i < KernelSize; i++)
        sum += texture(BaseImage, TexCoord.st + Offset[i]);
    FragColor = sum * ScaleFactor;
}

