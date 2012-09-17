#version {{ version }}

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;
uniform vec3 LightPosition;
uniform vec3 SkyColor;
uniform vec3 GroundColor;

in vec4 MCVertex;
in vec3 MCNormal;
in float Accessibility;

out vec3  DiffuseColor;

void main()
{
    vec3 ecPosition = vec3(MVMatrix * MCVertex);
    vec3 tnorm      = normalize(NormalMatrix * MCNormal);
    vec3 lightVec   = normalize(LightPosition - ecPosition);
    float costheta  = dot(tnorm, lightVec);
    float a         = 0.5 + 0.5 * costheta;
    DiffuseColor = mix(GroundColor, SkyColor, a) * Accessibility;
    gl_Position     = MVPMatrix * MCVertex;
}

