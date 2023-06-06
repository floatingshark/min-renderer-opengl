#include "render/render_object.hpp"

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

RenderObject::RenderObject()
{
	shape = std::make_shared<RenderShape>();
	shader = std::make_shared<ShadingBlinnPhong>();

	refreshShape();
}

RenderObject::~RenderObject()
{
}

void RenderObject::update()
{
	calculateModelMatrix();

	// model matrix
	shader->setModelMatrix(model_matrix);

	shader->update();
	shader->draw();
}

void RenderObject::refreshShape()
{
	shader->setShapeVertexPosition(shape->getVertexPosition());
	shader->setShapeVertexNormal(shape->getVertexNormal());
	shader->setShapeVertexIndex(shape->getVertexIndex());
	shader->setShapeVertexUv(shape->getVertexUv());
	shader->setShapeVertexTangent(shape->getTangent());
}

void RenderObject::calculateModelMatrix()
{
	const glm::mat4 translate_matrix = glm::translate(glm::mat4(1.f), glm::vec3(position[0], position[1], position[2]));
	const glm::mat4 rotate_x_matrix = glm::rotate(glm::mat4(1.f), rotation[0], glm::vec3(1.f, 0.f, 0.f));
	const glm::mat4 rotate_y_matrix = glm::rotate(glm::mat4(1.f), rotation[1], glm::vec3(0.f, 1.f, 0.f));
	const glm::mat4 rotate_z_matrix = glm::rotate(glm::mat4(1.f), rotation[2], glm::vec3(0.f, 0.f, 1.f));
	const glm::mat4 rotate_matrix = rotate_z_matrix * rotate_y_matrix * rotate_x_matrix;
	const glm::mat4 scale_matrix = glm::scale(glm::mat4(1.f), glm::vec3(scale[0], scale[1], scale[2]));
	model_matrix = translate_matrix * rotate_matrix * scale_matrix * glm::mat4(1.f);
}
