#version 430

in vec4 v_out_Position_World;
in vec4 v_out_Position_View;
in vec3 v_out_Normal_World;
in vec2 v_out_UV;
in mat3 v_out_TBN;

const uint u_Light = 1;
uniform vec3 u_CameraPosition;
uniform vec4 u_LightPosition[u_Light];
uniform vec3 u_LightAmbient[u_Light];
uniform sampler2D u_Texture;
uniform sampler2D u_TextureNormal;

uniform vec3 u_f3_material_ambient;
uniform vec3 u_f3_material_diffusion;
uniform vec3 u_f3_material_specular;
uniform float u_f_material_shiness;

out vec4 f_out_f4_fragment;

void main()
{   
	// normal map(tangent space) to world space
	vec3 NormalMap_Tangent = vec3(texture(u_TextureNormal, v_out_UV) * 2.0 - 1.0);
	vec3 NormalMap_World = v_out_TBN * NormalMap_Tangent;

	// eye position in world space (not tangent)
	vec3 ViewFromVertex= normalize(u_CameraPosition - vec3(v_out_Position_World));

	// calculate fragment
	vec3 DiffusionSum = vec3(0.0);
	vec3 SpecularSum = vec3(0.0);
	for (int i = 0; i < u_Light; ++i)
	{
		// light position in world space (not tangent)
		vec3 LightFromVertex = normalize((u_LightPosition[i] * v_out_Position_View.w - v_out_Position_View * u_LightPosition[i].w).xyz);

		// ambient & difusion
		vec3 Ambient = u_f3_material_ambient * u_LightAmbient[i];
		DiffusionSum += max(dot(NormalMap_World, LightFromVertex), 0.0) * u_f3_material_diffusion + Ambient;
		
		// specular
		vec3 HalfVector = normalize(LightFromVertex + ViewFromVertex);
	    SpecularSum += pow(max(dot(normalize(NormalMap_World), HalfVector), 0.0), u_f_material_shiness) * u_f3_material_specular;
	}

	// sum light and multiply texture
	DiffusionSum = DiffusionSum * vec3(texture(u_Texture, v_out_UV));
	f_out_f4_fragment = vec4(DiffusionSum + SpecularSum, 1.0);
	
	// for debug
	//f_out_f4_fragment = vec4(1.0, 1.0, 1.0, 1.0);
}