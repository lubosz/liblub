/**
* uniform variables
*

*/
float mx = 0, my = 0;
float ks = 0, kd = 1;
float BaseRefl = 0.7, A = 0;

/**
* Illumination function
*/
vec4 specularColor = vec4(0.5);
int specularU = 10;
int specularV = 10;
const float pi8 = 8 * 3.14159265358979323846264;

vec4 fresnelSchlick(vec3 k, vec3 h) {
    return specularColor + (1 - specularColor) * pow(1 - dot(k, h), 5);
}

vec4 ashikhminSpecular(vec3 normal, vec3 h) {
    float fac1 = sqrt(float((specularU + 1) * (specularV + 1))) / pi8;
    float fac2 = pow(dot(normal, h), 1);
    return specularColor;
}

vec4 AshikhminShirley( vec3 vLightWS, vec3 vViewWS, vec3 vNormalWS, vec4 cTexel, mat3 TBN)
{
    vec3 vHalf = normalize( -vViewWS + (-vLightWS) );     
    float PI = 3.1415926;

    float NdotV  = max ( 0.0, dot ( vNormalWS, -vViewWS ) );
    float NdotL  = max ( 0.0, dot ( vNormalWS, -vLightWS ) );
    float NdotH  = max ( 0.0, dot ( vNormalWS, vHalf ) );
    float HdotL  = max ( 0.0, dot ( vHalf, -vLightWS ) );
    float t1h = dot ( TBN[0], vHalf );
    float t2h = dot ( TBN[1], vHalf );
    //
    //vec3 Rd = cDiffuse;
    //vec3 Rs = 0.3f;
 
    //float Nu = fAnisotropy.x;
    //float Nv = fAnisotropy.y;
 
    //// Compute the diffuse term
    //vec3 Pd = (28.0f * Rd) / ( 23.0f * 3.14159f );
    //Pd *= (1.0f - Rs);
    //Pd *= (1.0f - pow(1.0f - (LdotN / 2.0f), 5.0f));
    //Pd *= (1.0f - pow(1.0f - (VdotN / 2.0f), 5.0f));
 
    //// Compute the specular term
    //float ps_num_exp = Nu * HdotT * HdotT + Nv * HdotB * HdotB;
    //ps_num_exp /= (1.0f - HdotN * HdotN);
 
    //float Ps_num = sqrt( (Nu + 1) * (Nv + 1) );
    //Ps_num *= pow( HdotN, ps_num_exp );
 
    //float Ps_den = 8.0f * 3.14159f * HdotL;
    //Ps_den *= max( LdotN, VdotN );
 
    //vec3 Ps = Rs * (Ps_num / Ps_den);
    //Ps *= ( Rs + (1.0f - Rs) * pow( 1.0f - HdotL, 5.0f ) );
 
    //// Composite the final value:
    //return vec4( Pd + Ps, 1.0f );
                        // calculate diffuse %(
    float rd = (28.0/(23.0*PI)) * ( 1.0 - pow ( 1.0 - 0.5*NdotV, 5.0 ) ) * ( 1.0 - pow (1.0 - 0.5*NdotL, 5.0) );
    
                        // calculate specular
    float B  = pow ( NdotH, (mx * t1h * t1h + my * t2h * t2h)/(1.0 - NdotH * NdotH) );
    float F  = ( BaseRefl + (1.0 - BaseRefl) * pow ( 1.0 - HdotL, 5.0 ) ) / ( HdotL * max ( NdotV, NdotL ) );
    float rs = B * F; // *A
    
    //vec4 diffuse = cTexel * kd * ( 1.0 - ks ) * rd;
    //vec4 specular = specularColor * ks * rs;
    vec4 diffuse = cTexel * rd;
    vec4 specular = specularColor * rs;

    vec4 color = NdotL * (diffuse + specular);
    return vec4(color);
}