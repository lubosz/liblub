#version 330
precision highp float;

in vec2 uv;
in float vEyeDist;
//uniform sampler2D sTexture;
float width = 0.1; // the width of the fog band
uniform float uFogPos; // the position of the fog band

out vec4 gl_FragColor;

void main() {
  // compute a gaussian curve
  float color = exp(-pow(vEyeDist - uFogPos, 2.0) / pow(2.0 * width, 2.0));
  gl_FragColor = vec4(0.0, color, color, 1.0);
  //texture2D(sTexture, vTextureCoord);
}
