#version 330 core

precision highp float;

in vec4 positionView;
in vec3 normalView;
in vec2 uv;

out vec4 finalColor;

uniform vec4 lightPositionView;

uniform vec4 lightColor;

//ambient
uniform vec4 ambientSceneColor;
//specular
uniform float shininess;
uniform vec4 specularMaterialColor;


uniform sampler2D diffuseTexture;

void main() { 

 	// Make sure the interpolated inputs and
    // constant parameters are normalized
    vec3 n = normalize( normalView );
    //vec3 l = normalize( -vLightDirection );
    vec3 l = -lightPositionView.xyz;
    vec3 v = positionView.xyz;
    //vec3 v = normalize( pCameraPosition - f.world );
    vec3 h = normalize( l + v );
 
    // Define the coordinate frame
    vec3 epsilon = vec3( 1.0f, 0.0f, 0.0f );
    vec3 tangent = normalize( cross( n, epsilon ) );
    vec3 bitangent = normalize( cross( n, tangent ) );
 
    // Generate any useful aliases
    float VdotN = dot( v, n );
    float LdotN = dot( l, n );
    float HdotN = dot( h, n );
    float HdotL = dot( h, l );
    float HdotT = dot( h, tangent );
    float HdotB = dot( h, bitangent );
 
    vec3 Rd = texture(diffuseTexture, uv).xyz;
    vec3 Rs = vec3(0.3f);
 
    //float Nu = fAnisotropy.x;
    //float Nv = fAnisotropy.y;
 	float Nu = .1;
 	float Nv = .1;
 
    // Compute the diffuse term
    vec3 Pd = (28.0f * Rd) / ( 23.0f * 3.14159f );
    Pd *= (1.0f - Rs);
    Pd *= (1.0f - pow(1.0f - (LdotN / 2.0f), 5.0f));
    Pd *= (1.0f - pow(1.0f - (VdotN / 2.0f), 5.0f));
 
    // Compute the specular term
    float ps_num_exp = Nu * HdotT * HdotT + Nv * HdotB * HdotB;
    ps_num_exp /= (1.0f - HdotN * HdotN);
 
    float Ps_num = sqrt( (Nu + 1) * (Nv + 1) );
    Ps_num *= pow( HdotN, ps_num_exp );
 
    float Ps_den = 8.0f * 3.14159f * HdotL;
    Ps_den *= max( LdotN, VdotN );
 
    vec3 Ps = Rs;
    //* (Ps_num / Ps_den);
    Ps *=  ( Rs + (vec3(1.0f) - Rs) * pow( 1.0f - HdotL, 2.0f ) );
 
    // Composite the final value:
    finalColor= vec4( Pd + Ps, 1.0f );
    
   // finalColor = vec4(Ps, 1.0f);

			
} 

