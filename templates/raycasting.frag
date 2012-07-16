#version 330 core

uniform sampler2D backfaces;
uniform sampler2D frontfaces;
uniform sampler3D volume;
uniform float stepsize;

in vec4 viewPosition;
in vec2 texCoord;
in vec4 thePosition;

out vec4 fragColor;

void main(void) {
    
  vec2 texc = ((viewPosition.xy / viewPosition.w) + 1) / 2; // find the right place to lookup in the backside buffer
  vec4 start = texture(frontfaces, texc); // the start position of the ray is stored in the texturecoordinate
  vec4 back_position  = texture(backfaces, texc);
  vec4 dir = back_position - start;
  float len = length(dir.xyz); // the length from front to back is calculated and used to terminate the ray
  
  if (len != 0) {
    vec3 norm_dir = normalize(dir.xyz);
    float delta = stepsize;
    vec3 delta_dir = norm_dir * delta;
    float delta_dir_len = length(delta_dir);
    vec3 vec = start.xyz;
    vec4 col_acc = vec4(0,0,0,0);
    float alpha_acc = 0;
    float length_acc = 0;
    vec4 color_sample;
    float alpha_sample;

    for(int i = 0; i < 450; i++) {
        color_sample = texture(volume,vec);
        if (color_sample != vec4(0)) {
          col_acc = color_sample;
          break;
        }
        
        alpha_sample = color_sample.a * stepsize;
        col_acc   += (1.0 - alpha_acc) * color_sample * alpha_sample * 3;
        alpha_acc += alpha_sample;
        vec += delta_dir;
        length_acc += delta_dir_len;
        
        
        if(length_acc >= len || alpha_acc > 1.0) {
          break; // terminate if opacity > 1 or the ray is outside the volume
        }
      }
   
    fragColor =  col_acc;
  } else {
    fragColor = vec4(0);
  }
  //fragColor = texture(frontfaces, texc);
  //fragColor = vec4(dir,1);
}
