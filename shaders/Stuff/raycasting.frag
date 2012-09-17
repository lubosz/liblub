#version {{ version }}

uniform sampler2D backfaces;
uniform sampler2D frontfaces;
uniform sampler3D volume;
uniform float stepsize;

precision highp float;

in vec4 viewPosition;
in vec2 texCoord;
in vec4 thePosition;
in vec4 lightPositionView;

out vec4 fragColor;

void main(void) {
    
  vec2 texc = ((viewPosition.xy / viewPosition.w) + 1) / 2; // find the right place to lookup in the backside buffer
  vec4 start = texture(frontfaces, texc); // the start position of the ray is stored in the texturecoordinate
  vec4 back_position  = texture(backfaces, texc);
  vec4 dir = back_position - start;
  float len = length(dir); // the length from front to back is calculated and used to terminate the ray
  vec3 delta_dir = vec3(0);
  //if(true) {
  if (len != 0) {
    vec3 norm_dir = normalize(dir.xyz);
    float delta = stepsize;
    delta_dir = norm_dir * delta;
    float delta_dir_len = length(delta_dir);
    vec3 vec = start.xyz;
    vec4 col_acc = vec4(0,0,0,0);
    float alpha_acc = 0;
    float length_acc = 0;
    vec4 color_sample;
    float alpha_sample;

    for (float i = 0; i < 9.0/stepsize; i++) {
    //for(int i = 0; i < 450; i++) {
        color_sample = texture(volume,vec);
        if (length(color_sample) > 1.0) {
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
   
   vec4 gradSample1 = texture(volume,vec + vec3(0,stepsize,0));
   vec4 gradSample2 = texture(volume,vec + vec3(0,-stepsize,0));
   vec4 gradSample3 = texture(volume,vec + vec3(stepsize,0,0));
   vec4 gradSample4 = texture(volume,vec + vec3(-stepsize,0,0));
   vec4 gradSample5 = texture(volume,vec + vec3(0,0,stepsize));
   vec4 gradSample6 = texture(volume,vec + vec3(0,0,-stepsize));
   
   vec4 normalColor = vec4(
     length(gradSample2) - length(gradSample1),
     length(gradSample4) - length(gradSample3),
     length(gradSample6) - length(gradSample5),
   1);
   
   fragColor = vec4(0);
   
   //normalColor = normalize(normalColor);
   
        vec3 lightDirection = lightPositionView.xyz - viewPosition.xyz;
        vec3 L = normalize(lightDirection);
//        vec3 N = normalize(normal);
        vec3 N = normalize(normalColor.xyz);
        float lambertTerm = max( dot(N,L), 0.0);

        if(lambertTerm > 0.0)	{
//            fragColor += diffuseColor(lambertTerm);
            fragColor += col_acc * lambertTerm;
                vec3 E = normalize(-viewPosition.xyz);
                vec3 R = reflect(-L, N);

                float specular = pow( max(dot(R, E), 0.0), 20 );

            fragColor += specular * vec4(1,1,1,1);
        }
   
   
    //fragColor =  col_acc;
    //fragColor = vec4(vec,1);
    //fragColor = normalColor;
  } else {
    fragColor = vec4(0);
  }
  //fragColor = dir;
  //fragColor = start;
  //fragColor = texture(frontfaces, texc);
  //fragColor = vec4(dir,1);
}
