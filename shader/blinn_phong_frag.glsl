#version 430
#extension GL_ARB_explicit_attrib_location: enable
precision mediump float;

const uint _ui_light_count = 1;

uniform vec3 u_f3_camera_position;
uniform vec4 u_f4_light_position[_ui_light_count];
uniform vec3 u_f3_light_ambient[_ui_light_count];
uniform vec3 u_f3_material_ambient;
uniform vec3 u_f3_material_diffusion;
uniform vec3 u_f3_material_specular;
uniform float u_f_material_shiness;
uniform sampler2D u_i_texture;
uniform sampler2D u_i_normal_texture;

in vec4 v_out_f4_position_world;
in vec4 v_out_f4_position_view;
in vec3 v_out_f3_normal_world;
in vec2 v_out_f2_uv;
in mat3 v_out_m3_tangent_to_world;
out vec4 f_out_f4_fragment;

void main()
{   
	// normal map(tangent space) to world space
	vec3 _v_f3_normal_map_tangent = vec3(texture2D(u_i_normal_texture, v_out_f2_uv) * 2.0 - 1.0);
	vec3 _v_f3_normal_map_world = v_out_m3_tangent_to_world * _v_f3_normal_map_tangent;

	// eye position in world space (not tangent)
	vec3 _v_f3_view_from_vertex_world= normalize(u_f3_camera_position - vec3(v_out_f4_position_world));

	// calculate fragment
	vec3 _v_f3_total_diffusion = vec3(0.0);
	vec3 _v_f3_total_specular = vec3(0.0);
	for (int i = 0; i < _ui_light_count; ++i)
	{
		// light position in world space (not tangent)
		vec3 _v_f3_light_from_vertex_world = normalize((u_f4_light_position[i] * v_out_f4_position_view.w - v_out_f4_position_view * u_f4_light_position[i].w).xyz);

		// ambient & difusion
		vec3 _f3_ambient = u_f3_material_ambient * u_f3_light_ambient[i];
		_v_f3_total_diffusion += max(dot(_v_f3_normal_map_world, _v_f3_light_from_vertex_world), 0.0) * u_f3_material_diffusion + _f3_ambient;
		
		// specular
		vec3 _v_f3_phong = normalize(_v_f3_light_from_vertex_world + _v_f3_view_from_vertex_world);
	    _v_f3_total_specular += pow(max(dot(normalize(_v_f3_normal_map_world), _v_f3_phong), 0.0), u_f_material_shiness) * u_f3_material_specular;
	}

	// sum light and multiply texture
	_v_f3_total_diffusion = _v_f3_total_diffusion * vec3(texture2D(u_i_texture, v_out_f2_uv));
	f_out_f4_fragment = vec4(_v_f3_total_diffusion + _v_f3_total_specular, 1.0);
	
	// for debug
	//f_out_f4_fragment = vec4(1.0, 1.0, 1.0, 1.0);
}