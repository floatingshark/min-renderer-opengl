#version 430

in vec4 v_in_Position;
in vec3 v_in_Normal;
in vec2 v_in_UV;
in vec3 v_in_Tangent;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat3 u_RotateNormalMatrix;

out vec4 v_out_Position;
out vec3 v_out_Normal;
out vec2 v_out_UV;
out mat3 v_out_TBN;

void main()
{
	v_out_Position = v_in_Position;
	v_out_UV = v_in_UV;

	v_out_Normal = normalize(u_RotateNormalMatrix * v_in_Normal);
	vec3 Tangent = normalize(u_RotateNormalMatrix * v_in_Tangent);
	vec3 Binormal = normalize(cross(v_out_Normal, Tangent));
	v_out_TBN = mat3(Tangent, Binormal, v_out_Normal);

	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * v_in_Position;
}