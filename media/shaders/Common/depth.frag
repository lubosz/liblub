#version 330 core

out vec4 gl_FragColor;

in vec4 viewPosition;

void main(void) {
    gl_FragColor = vec4(1,0,0,1);
    //gl_FragColor.a = max(min((-viewPosition.z)/20.0,1.0),0.0);
    //gl_FragDepth = max(min((-viewPosition.z)/20.0,1.0),0.0);
    //gl_FragDepth = 100;//viewPosition.z;
    gl_FragDepth = abs(viewPosition.z / 30);
    //gl_FragDepth = viewPosition.z / 30;
   // gl_FragDepth = 0.1;
}
