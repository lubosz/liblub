#version 330
precision highp float;

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform vec4 lightPositionView;

in vec3 in_Vertex;
in vec3 in_Normal;
in vec3 in_Tangent;
in vec2 in_Uv;

out vec2 uv;
out float vEyeDist;

void main() {
  vec4 bPosition = MVMatrix * vec4(in_Vertex,1.0);
  gl_Position = MVPMatrix * vec4(in_Vertex,1.0);
  uv = in_Uv;
  vEyeDist = sqrt((bPosition.x - lightPositionView.x) * (bPosition.x - lightPositionView.x)
  				+ (bPosition.y - lightPositionView.y) * (bPosition.y - lightPositionView.y)
  				+ (bPosition.z - lightPositionView.z) * (bPosition.z - lightPositionView.z));
}
