#version 410
//uniform sampler2D terrain;
uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform vec2 screen_size;

in vec3 in_Vertex;
in vec3 in_Normal;
out _{
vec4 world;
    vec3 eye;
    float dist;
    vec3 eye_normal;
    vec4 device;
    vec2 screen;
    bool offscreen;
    vec3 normal;
} o;

void main(void){
    vec2 texcoord = in_Vertex.xy;
    o.world = vec4(in_Vertex,1.0);
    o.eye = (MVMatrix * o.world).xyz;
    o.dist = length(o.eye);
    o.eye_normal = normalize(o.eye);
    o.device = MVPMatrix * o.world;
    o.device = clamp(o.device/o.device.w, -1.6, 1.6);
    o.screen = (o.device.xy+1) * (screen_size*0.5);
    o.normal = in_Normal;

    if(o.device.z < -0.5){
    	o.offscreen = true;
    }else{
        o.offscreen = any(
        	lessThan(o.device.xy, vec2(-1.59)) ||
        	greaterThan(o.device.xy, vec2(1.59))
        );
    }
}
