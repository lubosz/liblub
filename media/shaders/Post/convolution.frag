#version 150 core

// maximum size supported by this shader
const int MaxKernelSize = 25;

// array of offsets for accessing the base image
uniform vec2 Offset[MaxKernelSize];
// size of kernel (width * height) for this execution
uniform int KernelSize;
// value for each location in the convolution kernel
uniform vec4 KernelValue[MaxKernelSize];
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
    {
        vec4 tmp = texture(BaseImage, TexCoord.st + Offset[i]);
        sum += tmp * KernelValue[i];
    }
    FragColor = sum;
}

