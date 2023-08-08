#include "shading/shading_base.hpp"
#include "render/render_utility.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/matrix_operation.hpp"

void ShadingBase::update()
{
}

void ShadingBase::draw() const
{
	switch (draw_type)
	{
	case DrawType::Points:
		drawPoints();
		break;
	case DrawType::Lines:
		drawLines();
		break;
	case DrawType::Triangles:
		drawTriangles();
		break;
	default:
		break;
	}
}

void ShadingBase::setBaseColor(TextureType tex_type, const std::vector<unsigned char> base_color)
{
	removeTexture(tex_type);

	switch (tex_type)
	{
	case TextureType::Surface:
		texture_id = RenderUtility::generateTexture(base_color);
		break;
	case TextureType::Normal:
		normal_texture_id = RenderUtility::generateTexture(base_color);
		break;
	default:
		break;
	}
}

void ShadingBase::setTexture(TextureType tex_type, const char *texture_name)
{
	removeTexture(tex_type);
	std::vector<unsigned char> texture_data = RenderUtility::loadTexturePng(texture_name);
	switch (tex_type)
	{
	case TextureType::Surface:
		texture_id = RenderUtility::generateTexture(texture_data);
		break;
	case TextureType::Normal:
		normal_texture_id = RenderUtility::generateTexture(texture_data);
		break;
	default:
		break;
	}
}

void ShadingBase::removeTexture(TextureType tex_type)
{
	switch (tex_type)
	{
	case TextureType::Surface:
		RenderUtility::unbindTexture(texture_id);
		break;
	case TextureType::Normal:
		RenderUtility::unbindTexture(normal_texture_id);
		break;
	default:
		break;
	}
}

void ShadingBase::setShaderProgram(const char *vert_shader_name, const char *frag_shader_name)
{
	shader_program = RenderUtility::loadShaderProgram(vert_shader_name, frag_shader_name);
}

void ShadingBase::calculateAttribute()
{
}

void ShadingBase::assignAttributeVariables()
{
}

void ShadingBase::initializeUniformVariables()
{
	texture_id = 0;
	normal_texture_id = 0;
	setBaseColor(TextureType::Surface, {255, 255, 255});
	setBaseColor(TextureType::Normal, {128, 128, 255});
}

void ShadingBase::initializeUniformLocations()
{
	model_matrix_location = glGetUniformLocation(shader_program, "u_ModelMatrix");
	view_matrix_location = glGetUniformLocation(shader_program, "u_ViewMatrix");
	projection_matrix_location = glGetUniformLocation(shader_program, "u_ProjectionMatrix");
	rotate_normal_matrix_location = glGetUniformLocation(shader_program, "u_RotateNormalMatrix");

	camera_position_location = glGetUniformLocation(shader_program, "u_CameraPosition");
	light_position_location = glGetUniformLocation(shader_program, "u_LightPosition");
	light_ambient_location = glGetUniformLocation(shader_program, "u_LightAmbient");

	texture_location = glGetUniformLocation(shader_program, "u_Texture");
	normal_texture_location = glGetUniformLocation(shader_program, "u_TextureNormal");
}

void ShadingBase::assignUniformVariables()
{
	// todo
	projection_matrix = glm::perspective(glm::radians(60.f), 4.f / 3.f, 0.1f, 100.f);

	const glm::mat3 m4_matrix = glm::mat3(model_matrix);
	rotate_normal_matrix = glm::transpose(glm::adjugate(m4_matrix));

	glUniformMatrix4fv(model_matrix_location, 1, GL_FALSE, &model_matrix[0][0]);
	glUniformMatrix4fv(view_matrix_location, 1, GL_FALSE, &view_matrix[0][0]);
	glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE, &projection_matrix[0][0]);
	glUniformMatrix3fv(rotate_normal_matrix_location, 1, GL_FALSE, &rotate_normal_matrix[0][0]);

	glUniform3fv(camera_position_location, 1, camera_position.data());
	glUniform4fv(light_position_location, 1, light_position.data());
	glUniform3fv(light_ambient_location, 1, light_ambient.data());

	glUniform1i(texture_location, 0);
	glUniform1i(normal_texture_location, 1);
}

void ShadingBase::drawPoints() const
{
	glDrawElements(GL_POINTS, vertex_index.size(), GL_UNSIGNED_INT, 0);
}

void ShadingBase::drawLines() const
{
	glDrawElements(GL_LINE_STRIP, vertex_index.size(), GL_UNSIGNED_INT, 0);
}

void ShadingBase::drawTriangles() const
{
	glDrawElements(GL_TRIANGLES, vertex_index.size(), GL_UNSIGNED_INT, 0);
}

void ShadingBase::bindContext() const
{
	// 使用するvaoを指定する
	glBindVertexArray(vertex_array_object);
}

void ShadingBase::errorCheck() const
{
	// error checker
	auto err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::cerr << "OpenGL error: " << err << std::endl;
	}
}