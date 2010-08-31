#version 330
precision highp float;

uniform mat4 MVPMatrix;
uniform vec3 LightPosition;

in vec3 in_Vertex;
in vec2 in_Uv;

out vec2 uv;
out float vEyeDist;

void main() {
  vec4 bPosition = MVPMatrix * vec4(in_Vertex,1.0);
  gl_Position = bPosition;
  uv = in_Uv;
  vEyeDist = sqrt((bPosition.x - LightPosition.x) * (bPosition.x - LightPosition.x)
  				+ (bPosition.y - LightPosition.y) * (bPosition.y - LightPosition.y)
  				+ (bPosition.z - LightPosition.z) * (bPosition.z - LightPosition.z));
}
