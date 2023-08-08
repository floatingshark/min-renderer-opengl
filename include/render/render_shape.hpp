#pragma once
#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "render/render_utility.hpp"

enum class ShapeType
{
	Plane,
	Cube,
	Sphere
};

class RenderShape
{
public:
	RenderShape();
	~RenderShape(){};
	RenderShape(RenderShape const &) = delete;
	RenderShape &operator=(RenderShape const &) = delete;

	inline ShapeType getShapeType() const { return shape_type; };
	inline void setShapeType(const ShapeType new_type) {shape_type = new_type;};

	std::vector<GLfloat> getVertexPosition() const { return RenderUtility::flattenVector(vertex_position); };
	std::vector<GLfloat> getVertexNormal() const { return RenderUtility::flattenVector(vertex_normal); };
	std::vector<GLuint> getVertexIndex() const { return RenderUtility::flattenVector(vertex_index); };
	std::vector<GLfloat> getVertexUv() const { return RenderUtility::flattenVector(vertex_uv); };
	std::vector<GLfloat> getTangent() const { return RenderUtility::flattenVector(vertex_tangent); };

	void makePlaneVertex();
	void makeCubeVertex();
	void makeSphereVertex();

protected:
	ShapeType shape_type = ShapeType::Cube;

	std::vector<std::vector<GLfloat>> vertex_position;
	std::vector<std::vector<GLfloat>> vertex_normal;
	std::vector<std::vector<GLuint>> vertex_index;
	std::vector<std::vector<GLfloat>> vertex_uv;
	std::vector<std::vector<GLfloat>> vertex_tangent;

	std::vector<std::vector<GLfloat>> calculateTangent();

	// サンプル形状
	const std::vector<std::vector<GLfloat>> plane_position = {
		{1.0f, 1.0f, 0.0f},
		{-1.0f, 1.0f, 0.0f},
		{-1.0f, -1.0f, 0.0f},
		{1.0f, -1.0f, 0.0f},
	};
	const std::vector<std::vector<GLfloat>> plane_normal = {
		{0.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 1.0f},
	};
	const std::vector<std::vector<GLuint>> plane_index = {
		{0, 1, 2},
		{0, 2, 3}};
	const std::vector<std::vector<GLfloat>> plane_uv = {
		{1.f, 1.f},
		{0.f, 1.f},
		{0.f, 0.f},
		{1.f, 0.f}};

	const std::vector<std::vector<GLfloat>> cube_position = {
		// 左
		{-1.0f, -1.0f, -1.0f},
		{-1.0f, -1.0f, 1.0f},
		{-1.0f, 1.0f, 1.0f},
		{-1.0f, 1.0f, -1.0f},
		// 裏
		{1.0f, -1.0f, -1.0f},
		{-1.0f, -1.0f, -1.0f},
		{-1.0f, 1.0f, -1.0f},
		{1.0f, 1.0f, -1.0f},
		// 下
		{-1.0f, -1.0f, -1.0f},
		{1.0f, -1.0f, -1.0f},
		{1.0f, -1.0f, 1.0f},
		{-1.0f, -1.0f, 1.0f},
		// 右
		{1.0f, -1.0f, 1.0f},
		{1.0f, -1.0f, -1.0f},
		{1.0f, 1.0f, -1.0f},
		{1.0f, 1.0f, 1.0f},
		// 上
		{-1.0f, 1.0f, -1.0f},
		{-1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, -1.0f},
		// 前
		{-1.0f, -1.0f, 1.0f},
		{1.0f, -1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f},
		{-1.0f, 1.0f, 1.0f}};
	const std::vector<std::vector<GLfloat>> cube_normal = {
		// 左
		{-1.0f, 0.0f, 0.0f},
		{-1.0f, 0.0f, 0.0f},
		{-1.0f, 0.0f, 0.0f},
		{-1.0f, 0.0f, 0.0f},
		// 裏
		{0.0f, 0.0f, -1.0f},
		{0.0f, 0.0f, -1.0f},
		{0.0f, 0.0f, -1.0f},
		{0.0f, 0.0f, -1.0f},
		// 下
		{0.0f, -1.0f, 0.0f},
		{0.0f, -1.0f, 0.0f},
		{0.0f, -1.0f, 0.0f},
		{0.0f, -1.0f, 0.0f},
		// 右
		{1.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 0.0f},
		// 上
		{0.0f, 1.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		// 前
		{0.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 1.0f}};
	const std::vector<std::vector<GLuint>> cube_index = {
		// 左
		{0, 1, 2},
		{0, 2, 3},
		// 裏
		{4, 5, 6},
		{4, 6, 7},
		// 下
		{8, 9, 10},
		{8, 10, 11},
		// 右
		{12, 13, 14},
		{12, 14, 15},
		// 上
		{16, 17, 18},
		{16, 18, 19},
		// 前
		{20, 21, 22},
		{20, 22, 23}};
	const std::vector<std::vector<GLfloat>> cube_uv = {
		// 左
		{1.f, 1.f},
		{0.f, 1.f},
		{0.f, 0.f},
		{1.f, 0.f},
		// 裏
		{1.f, 1.f},
		{0.f, 1.f},
		{0.f, 0.f},
		{1.f, 0.f},
		// 下
		{1.f, 1.f},
		{0.f, 1.f},
		{0.f, 0.f},
		{1.f, 0.f},
		// 右
		{1.f, 1.f},
		{0.f, 1.f},
		{0.f, 0.f},
		{1.f, 0.f},
		// 上
		{1.f, 1.f},
		{0.f, 1.f},
		{0.f, 0.f},
		{1.f, 0.f},
		// 前
		{1.f, 1.f},
		{0.f, 1.f},
		{0.f, 0.f},
		{1.f, 0.f}};
};