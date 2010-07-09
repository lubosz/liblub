uniform float Time;

void main() {

  gl_Position = gl_ModelViewMatrix * gl_Vertex;

  gl_Position.xyz = gl_Position.xyz + (0.25 * ( abs(sin(0.125*Time)) * gl_Normal));

  gl_Position = gl_ProjectionMatrix * gl_Position; 

  gl_FrontColor = gl_Color;

}