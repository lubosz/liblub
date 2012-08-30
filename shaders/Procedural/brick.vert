#version 330 core

in vec3 in_Position;
in vec3 in_Normal;

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;
uniform vec4 lightPositionView;

const float SpecularContribution = 0.3;
const float DiffuseContribution = 1.0 - SpecularContribution;
out float lightIntensity;
out vec2 positionModel;

void main()
{
    vec4 position = vec4(in_Position,1);
    vec3 positionView = vec3(MVMatrix * position);
    vec3 normalView = normalize(NormalMatrix * in_Normal);
    vec3 lightVec = normalize(lightPositionView.xyz - positionView);
    vec3 reflectVec = reflect(-lightVec, normalView);
    vec3 viewVec = normalize(-positionView);
    float diffuse = max(dot(lightVec, normalView), 0.0);
    float spec = 0.0;
    if (diffuse > 0.0)
    {
        spec = max(dot(reflectVec, viewVec), 0.0);
        spec = pow(spec, 16.0);
    }
    lightIntensity = DiffuseContribution * diffuse + SpecularContribution * spec;
    positionModel = position.xy;
    gl_Position = MVPMatrix * position;
}

