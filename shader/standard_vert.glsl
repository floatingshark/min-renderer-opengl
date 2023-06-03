#version 430
#extension GL_ARB_explicit_attrib_location: enable

uniform mat4 u_m4_model_matrix;
uniform mat4 u_m4_view_matrix;
uniform mat4 u_m4_projection_matrix;
uniform mat3 u_m4_rotate_normal_matrix;

in vec4 v_in_f4_position_local;
in vec3 v_in_f3_normal_local;
in vec2 v_in_f2_uv;
in vec3 v_in_f3_tangent_local;

out vec4 v_out_f4_position_world;
out vec4 v_out_f4_position_view;
out vec3 v_out_f3_normal_world;
out vec2 v_out_f2_uv;
out mat3 v_out_m3_tangent_to_world;

void main()
{
	// vertex matrix transformations
	v_out_f4_position_world = u_m4_model_matrix * v_in_f4_position_local;
	v_out_f4_position_view = u_m4_view_matrix * v_out_f4_position_world;
	v_out_f2_uv = v_in_f2_uv;

	// tangent space → world space matrix calculate
	v_out_f3_normal_world = normalize(u_m4_rotate_normal_matrix * v_in_f3_normal_local);
	vec3 _v_f3_tangent_world = normalize(u_m4_rotate_normal_matrix * v_in_f3_tangent_local);
	vec3 _v_f3_binormal_world = normalize(cross(v_out_f3_normal_world, _v_f3_tangent_world));
	v_out_m3_tangent_to_world = mat3(_v_f3_tangent_world, _v_f3_binormal_world, v_out_f3_normal_world);

	// set vertex position
	gl_Position = u_m4_projection_matrix * v_out_f4_position_view;
}