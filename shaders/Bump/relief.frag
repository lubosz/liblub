#version {{ version }}

in vec4 positionWS;
in vec3 normalWS;
in vec3 tangentWS;
in vec3 binormalWS;
in vec2 uv;

uniform sampler2D normalTexture;
uniform sampler2D diffuseTexture;
uniform vec3 lightPositionWS;
uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform vec3 camPositionWorld;

// tile factor
uniform float tile;
// scale factor for height-field depth
uniform float depth;

const int linearSearchSteps = 20;
const int binarySearchSteps = 10;

out vec4 fragColor;

float saturate(float value) {
    return clamp(value, 0.0, 1.0);
}

vec3 dot3(vec3 vector, vec3 t, vec3 b, vec3 n) {
    return normalize(vec3(
                dot(vector, t),
                dot(vector, b),
                dot(vector, n)
    ));
}

float rayIntersect(vec2 uvSearchPosition, vec2 uvSearchSize) {
    float depthStep = 1.0 / linearSearchSteps;
    // current size of search window
    float size = depthStep; 
    // current depth position
    float depthPosition = 0.0; 
    // best match found (starts with last position 1.0)
    float bestDepth = 1.0;
    // search from front to back for first point inside the object
    for (int i = 0; i < linearSearchSteps - 1; i++) {
        depthPosition += size;
        float textureDepth = texture(normalTexture, uvSearchPosition + uvSearchSize * depthPosition).w;
        // if no depth found yet
        if (bestDepth > 0.996) 
            if (depthPosition >= textureDepth)
                // store bestDepth
                bestDepth = depthPosition; 
    }
    depthPosition = bestDepth;
    // search around first point (depthPosition) for closest match
    for (int i = 0; i < binarySearchSteps; i++) {
        size *= 0.5;
        float textureDepth = texture(normalTexture, uvSearchPosition + uvSearchSize * depthPosition).w;
        if (depthPosition >= textureDepth) {
            bestDepth = depthPosition;
            depthPosition -= 2 * size;
        }
        depthPosition += size;
    }
    return bestDepth;
}

bool checkShadow(vec2 uvSearchPosition, vec2 uvSearchSize, vec3 lightDirectionTS, float intersectDistance) {
    // ray intersect in light direction
    // update position in texture space
    vec2 pos = uvSearchPosition + uvSearchSize * intersectDistance; 
    // light direction in texture space
    uvSearchSize = lightDirectionTS.xy * depth / lightDirectionTS.z;
    // entry point for light ray in texture space
    pos -= uvSearchSize * intersectDistance; 
    // get intresection distance from light ray
    float intersectDistanceLight = rayIntersect(pos, uvSearchSize);
    // if pixel in shadow
    return intersectDistanceLight < intersectDistance - 0.05;
}

void main() {
    // compute light direction
    vec3 lightDirectionWS = normalize(positionWS.xyz - lightPositionWS);
    // view vector in world space
    vec3 camDirectionWS = normalize(positionWS.xyz - camPositionWorld);
    // view vector in tangent space
    vec3 camDirectionTS = dot3(camDirectionWS, tangentWS, binormalWS, normalWS);
    vec3 lightDirectionTS = dot3(lightDirectionWS, tangentWS, binormalWS, normalWS);

    // ray intersect in view direction
    // size and start position of search in texture space
    vec2 uvSearchSize = camDirectionTS.xy * depth;
    vec2 uvSearchPosition = uv * tile;

    // get intersection distance
    float intersectDistance = rayIntersect(uvSearchPosition, uvSearchSize);


    // get normal and color at intersection point
    vec2 uv2 = uvSearchPosition + uvSearchSize * intersectDistance;
    vec3 normalTS = texture(normalTexture, uv2).xyz;
    vec4 diffuseTextureColor = texture(diffuseTexture, uv2);

    // expand normal
    normalTS = normalTS * 2.0 - 1.0;

    // compute diffuse and specular terms
    float diffuseTerm = saturate(dot(normalTS,-lightDirectionTS));

    vec3 reflectLight = reflect(lightDirectionTS, normalTS);
    float specularTerm = saturate(dot(reflectLight, -camDirectionTS));
    specularTerm = pow(specularTerm, specularColor.w);

    bool shadow = checkShadow(uvSearchPosition, uvSearchSize, lightDirectionTS, intersectDistance);
    if (shadow) {
        diffuseTerm *= dot(ambientColor, vec4(1)) * 0.3333;
        specularTerm = 0;
    }
    // apply shininess

    vec4 finalSpecular = specularColor * specularTerm;
    vec4 finalDiffuse = diffuseTextureColor * diffuseColor * diffuseTerm;

    float attenuation = saturate(dot(-lightDirectionWS, normalWS));
    attenuation = 1;

    fragColor = ambientColor * diffuseTextureColor;
    fragColor = attenuation * (finalDiffuse + finalSpecular);
}
