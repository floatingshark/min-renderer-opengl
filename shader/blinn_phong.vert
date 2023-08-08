#version 430

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat3 u_RotateNormalMatrix;

in vec4 v_in_Position;
in vec3 v_in_Normal;
in vec2 v_in_UV;
in vec3 v_in_Tangent;

out vec4 v_out_Position_World;
out vec4 v_out_Position_View;
out vec3 v_out_Normal_World;
out vec2 v_out_UV;
out mat3 v_out_TBN;

void main()
{
	// vertex matrix transformations
	v_out_Position_World = u_ModelMatrix * v_in_Position;
	v_out_Position_View = u_ViewMatrix * v_out_Position_World;
	v_out_UV = v_in_UV;

	// tangent space → world space matrix calculate
	v_out_Normal_World = normalize(u_RotateNormalMatrix * v_in_Normal);
	vec3 Tangent_World = normalize(u_RotateNormalMatrix * v_in_Tangent);
	vec3 Binormal_World = normalize(cross(v_out_Normal_World, Tangent_World));
	v_out_TBN = mat3(Tangent_World, Binormal_World, v_out_Normal_World);

	// set vertex position
	gl_Position = u_ProjectionMatrix * v_out_Position_View;
}