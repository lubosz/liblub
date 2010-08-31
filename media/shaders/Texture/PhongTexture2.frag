#version 330

uniform mat4 MVMatrix;
uniform sampler2D texture;

in vec3 position;
//in vec3 color;
in vec3 normal;
//in vec2 uv;

uniform vec3 LightPosition;
uniform float shininess;
uniform vec4 specular;

const vec4 light_diffuse = vec4(0.8, 0.8, 0.8, 0.0);
const vec4 light_ambient = vec4(0.2, 0.2, 0.2, 1.0);
const vec4 light_specular = vec4(1.0, 1.0, 1.0, 1.0);

out vec4 gl_FragColor;

void main()
{
    vec3 mv_light_direction = (MVMatrix * vec4(LightPosition, 0.0)).xyz,
         normal_normal = normalize(normal),
         //normal_normal = normal,
         eye = normalize(position),
         reflection = reflect(mv_light_direction, normal_normal);

    //vec4 diffuse = texture2D(texture, uv);
    //vec4 diffuse = vec4(normal,1.0);
    vec4 diffuse = vec4(1.0,1.0,1.0,1.0);
    vec4 diffuse_factor = max(-dot(normal_normal, mv_light_direction), 0.0) * light_diffuse;
    vec4 ambient_diffuse_factor = diffuse_factor + light_ambient;
    vec4 specular_factor = max(pow(-dot(reflection, eye), shininess), 0.0) * light_specular;
    
    gl_FragColor = specular_factor * specular
        + ambient_diffuse_factor * diffuse;
}
