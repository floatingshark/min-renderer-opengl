#version 430

in vec4 v_out_Position;
in vec3 v_out_Normal;
in vec2 v_out_UV;
in mat2 v_out_TBN;

uniform vec3 u_CameraPosition;
uniform vec4 u_LightPosition;
uniform vec3 u_LightAmbient;
uniform sampler2D u_Texture;
uniform sampler2D u_TextureNormal;

out vec4 f_Fragment;

const float PI = acos(-1);
const float INV_PI = 1 / PI;
const float TAU = PI * 2;

// D : Microfacet Distribution Functions
// GGX Trowbridge-Reitz model
float D_GGX(float perceptualRoughness, float ndoth, vec3 normal, vec3 halfDir) 
{
    vec3 ncrossh = cross(normal, halfDir);
    float a = ndoth * perceptualRoughness;
    float k = perceptualRoughness / (dot(ncrossh, ncrossh) + a * a);
    float d = k * k * INV_PI;
    return min(d, 65504.0);
}

// V : Geometorical attenuation
// Height-Correlated Smith
float V_Smith_GGX_Correlated(float ndotl, float ndotv, float alpha)
{
    float lambdaV = ndotl * (ndotv * (1 - alpha) + alpha);
    float lambdaL = ndotv * (ndotl * (1 - alpha) + alpha);
    return 0.5f / (lambdaV + lambdaL + 0.0001);
}

// F : Fresnel
// Schlick
vec3 F_Schlick(vec3 f0, float cos)
{
    return f0 + (1 - f0) * pow(1 - cos, 5);
}

void main()
{   
	// specular BRDF (Microfacet BRDF) Cook-Torrance model
	// D * G * F / 4 (l dot n)(v dot n) ≒ D * V * F



	// for debug
	f_Fragment = vec4(1.0, 1.0, 1.0, 1.0);
}