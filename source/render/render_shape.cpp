#include "render/render_shape.hpp"

#include <iostream>
#include <vector>
#include <cmath>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

RenderShape::RenderShape()
{
	// 初期形状を設定
	makeCubeVertex();
}

void RenderShape::setShapeType(const ShapeType new_type)
{
	shape_type = new_type;
	switch (shape_type)
	{
	case ShapeType::Plane:
		makePlaneVertex();
		break;
	case ShapeType::Cube:
		makeCubeVertex();
		break;
	case ShapeType::Sphere:
		makeSphereVertex();
		break;
	default:
		break;
	}
}

void RenderShape::makePlaneVertex()
{
	vertex_position = plane_position;
	vertex_normal = plane_normal;
	vertex_index = plane_index;
	vertex_uv = plane_uv;
	vertex_tangent = calculateTangent();
}

void RenderShape::makeCubeVertex()
{
	vertex_position = cube_position;
	vertex_normal = cube_normal;
	vertex_index = cube_index;
	vertex_uv = cube_uv;
	vertex_tangent = calculateTangent();
}

void RenderShape::makeSphereVertex()
{
	const int slices(16), stacks(8);
	std::vector<std::vector<GLfloat>> sphere_position;
	for (int j = 0; j <= stacks; ++j)
	{
		const float t(static_cast<float>(j) / static_cast<float>(stacks));
		const float y(cos(3.141593f * t)), r(sin(3.141593f * t));
		for (int i = 0; i <= slices; ++i)
		{
			const float s(static_cast<float>(i) / static_cast<float>(slices));
			const float z(r * cos(6.283185f * s)), x(r * sin(6.283185f * s));
			const std::vector<GLfloat> vert = {x, y, z};
			sphere_position.emplace_back((vert));
		}
	}

	std::vector<std::vector<GLuint>> sphere_index;
	for (int j = 0; j < stacks; ++j)
	{
		const int k((slices + 1) * j);
		for (int i = 0; i < slices; ++i)
		{
			const GLuint k0(k + i);
			const GLuint k1(k0 + 1);
			const GLuint k2(k1 + slices);
			const GLuint k3(k2 + 1);

			const std::vector<GLuint> vert_1 = {k0, k2, k3};
			sphere_index.emplace_back(vert_1);
			const std::vector<GLuint> vert_2 = {k0, k3, k1};
			sphere_index.emplace_back(vert_2);
		}
	}

	vertex_position = sphere_position;
	vertex_normal = sphere_position;
	vertex_index = sphere_index;
}

std::vector<std::vector<GLfloat>> RenderShape::calculateTangent()
{
	std::vector<std::vector<GLfloat>> tangent_vector(vertex_position.size());
	for (int i = 0; i < static_cast<int>(vertex_index.size()); i++)
	{
		std::vector<GLfloat> vertex_0 = vertex_position[vertex_index[i][0]];
		std::vector<GLfloat> vertex_1 = vertex_position[vertex_index[i][1]];
		std::vector<GLfloat> vertex_2 = vertex_position[vertex_index[i][2]];
		std::vector<GLfloat> uv_0 = vertex_uv[vertex_index[i][0]];
		std::vector<GLfloat> uv_1 = vertex_uv[vertex_index[i][1]];
		std::vector<GLfloat> uv_2 = vertex_uv[vertex_index[i][2]];

		std::vector<GLfloat> delta_pos1(3);
		std::vector<GLfloat> delta_pos2(3);
		std::vector<GLfloat> delta_uv1(2);
		std::vector<GLfloat> delta_uv2(2);
		for (int j = 0; j < 3; j++)
		{
			delta_pos1[j] = vertex_1[j] - vertex_0[j];
			delta_pos2[j] = vertex_2[j] - vertex_0[j];
		}
		for (int k = 0; k < 2; k++)
		{
			delta_uv1[k] = uv_1[k] - uv_0[k];
			delta_uv2[k] = uv_2[k] - uv_0[k];
		}

		std::vector<GLfloat> tangent(3);
		//std::vector<GLfloat> binormal(3);
		float r = 1.0f / (delta_uv1[0] * delta_uv2[1] - delta_uv2[0] * delta_uv1[1]);
		for (int l = 0; l < 3; l++)
		{
			tangent[l] = (delta_pos1[l] * delta_uv2[1] - delta_pos2[l] * delta_uv1[1]) * r;
			//binormal[l] = (-delta_pos1[l] * delta_uv2[0] + delta_pos2[l] * delta_uv1[0]) * r;
		}

		tangent_vector[vertex_index[i][0]] = tangent;
		tangent_vector[vertex_index[i][1]] = tangent;
		tangent_vector[vertex_index[i][2]] = tangent;
	}

	for (int i = 0; i < static_cast<int>(tangent_vector.size()); i++)
	{
		GLfloat sum = 0.f;
		for (int j = 0; j < 3; j++)
		{
			sum += std::pow(tangent_vector[i][j], 2.f);
		}
		sum = std::sqrt(sum);
		for (int j = 0; j < 3; j++)
		{
			tangent_vector[i][j] /= sum;
		}
	}
	return tangent_vector;
}