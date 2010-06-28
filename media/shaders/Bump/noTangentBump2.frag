#version 150 core

uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform sampler2D glossMap;

uniform float glossMax;
uniform float glossNoise;
uniform vec4 glossColor;

uniform vec4 LightAmbient;
uniform vec4 LightDiffuse;
uniform vec4 LightSpecular;
uniform vec4 MaterialDiffuse;
uniform vec4 MaterialSpecular;

in vec2 TexCoord;

in vec3 lightDir; 
in vec3 viewDir;
in vec3 normal;

out vec4 gl_FragColor;

//calculates tangent space matrix from normal, vector in plane and texture coordinates
//needs normalized normal and position!

mat3 computeTangentFrame(vec3 normal, vec3 position, vec2 TexCoord)
{
    vec3 dpx = dFdx(position);
    vec3 dpy = dFdy(position);
    vec2 dtx = dFdx(TexCoord);
    vec2 dty = dFdy(TexCoord);
    
    vec3 tangent = normalize(dpx * dty.t - dpy * dtx.t);
	vec3 binormal = normalize(-dpx * dty.s + dpy * dtx.s);
	//vec3 tangent = normalize(dpy * dtx.t - dpx * dty.t);
    //vec3 binormal = cross(tangent, normal);
   
    return mat3(tangent, binormal, normal);
}

float rand(vec2 coordinate) {
    return fract(sin(dot(coordinate.xy, vec2(12.9898,78.233))) * 43758.5453);
}

void main (void)
{
        vec3 v = normalize(viewDir);
        vec3 l = normalize(lightDir);
        vec3 n = normalize(normal);
        vec3 bump = normalize(texture2D(normalMap, TexCoord).xyz * 2.0 - 1.0);
        
        mat3 TBN = computeTangentFrame(n, v, TexCoord);        
        
        n = normalize(TBN * bump); 
        float nDotL = clamp(dot(l, n), 0.0, 1.0);
        
        vec4 ambient = LightAmbient;
        vec4 diffuse = LightDiffuse * MaterialDiffuse * nDotL;
        vec4 specular = vec4(0,0,0,0);
        vec4 glossAdditional = vec4(0,0,0,0);
        
        if (nDotL >= 0.0) { 
            vec4 glossFromMap = texture2D(glossMap, TexCoord);
            float glossMaskValue = (glossFromMap.x + glossFromMap.y + glossFromMap.z) * 0.333333;
            if (glossMaskValue > 0.0) {
               float nDotV = clamp(dot(reflect(-l, n), v), 0.0, 1.0);
               float glossAtPosition = clamp(glossMax * glossMaskValue, 0.0, glossMax);
               glossAtPosition += glossNoise * rand(TexCoord);
               specular = LightSpecular * MaterialSpecular * pow(nDotV, glossAtPosition);
               glossAdditional = nDotV * glossColor * glossAtPosition;
            }
        }
        
        //vec4 color = gl_FrontLightModelProduct.sceneColor + ambient + diffuse;
        vec4 color = ambient + diffuse;

        gl_FragColor = color * texture2D(colorMap, TexCoord) + specular + glossAdditional;    
}
