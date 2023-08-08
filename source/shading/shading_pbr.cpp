#include "shading/shading_pbr.hpp"
#include "render/render_utility.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/matrix_operation.hpp"

ShadingPBR::ShadingPBR()
{
    ShadingBase::setShaderProgram(vert_shader_name, frag_shader_name);

    initializeUniformVariables();
    initializeUniformLocations();
}

void ShadingPBR::update()
{
	ShadingBase::update();

	glUseProgram(shader_program);

	calculateAttribute();
	assignAttributeVariables();

	assignUniformVariables();

	bindContext();
}

void ShadingPBR::calculateAttribute()
{
	ShadingBase::calculateAttribute();
}

void ShadingPBR::assignAttributeVariables()
{
	ShadingBase::assignAttributeVariables();

	std::vector<GLfloat> vertex_context;
	vertex_context.insert(vertex_context.end(), vertex_position.begin(), vertex_position.end());
	vertex_context.insert(vertex_context.end(), vertex_normal.begin(), vertex_normal.end());
	vertex_context.insert(vertex_context.end(), vertex_uv.begin(), vertex_uv.end());
	vertex_context.insert(vertex_context.end(), vertex_tangent.begin(), vertex_tangent.end());

	glGenVertexArrays(1, &vertex_array_object);
	glBindVertexArray(vertex_array_object);

	glGenBuffers(1, &vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, vertex_context.size() * sizeof(GLfloat), &vertex_context[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(vertex_position.size() * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)((vertex_position.size() + vertex_normal.size()) * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)((vertex_position.size() + vertex_normal.size() + vertex_uv.size()) * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);

	glGenBuffers(1, &index_element_buffer_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_element_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertex_index.size() * sizeof(GLuint), &vertex_index[0], GL_STATIC_DRAW);
}

void ShadingPBR::initializeUniformVariables()
{
	ShadingBase::initializeUniformVariables();
}

void ShadingPBR::initializeUniformLocations()
{
	ShadingBase::initializeUniformLocations();
}

void ShadingPBR::assignUniformVariables()
{
	ShadingBase::assignUniformVariables();
}
