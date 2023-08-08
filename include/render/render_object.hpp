#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/fwd.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "render/render_shape.hpp"
#include "shading/shading_base.hpp"

/**
 * each object in renderer
 */
class RenderObject
{
public:
	RenderObject();
	~RenderObject();
	RenderObject(RenderObject const &) = delete;
	RenderObject &operator=(RenderObject const &) = delete;

	void update();
	void refreshShape();
	void switchShader(ShaderType nex_shader);
	void switchShape(ShapeType new_shape);

	inline std::shared_ptr<RenderShape> getShape() { return shape; };
	inline std::shared_ptr<ShadingBase> getShader() { return shader; };

	inline std::vector<float> getPosition() const { return position; };
	inline void setPosition(const std::vector<float> new_position) { position = new_position; };
	inline std::vector<float> getRotation() const { return rotation; };
	inline void setRotation(const std::vector<float> new_rotation) { rotation = new_rotation; };
	inline std::vector<float> getScale() const { return scale; };
	inline void setScale(const std::vector<float> new_scale) { scale = new_scale; };

	inline std::string getName() const { return name; };
	inline void setName(const char *new_name) { name = std::string(new_name); };

protected:
	std::shared_ptr<RenderShape> shape;
	std::shared_ptr<ShadingBase> shader;

	std::vector<float> position = {0.f, 0.f, 0.f};
	std::vector<float> rotation = {0.f, 0.f, 0.f};
	std::vector<float> scale = {1.f, 1.f, 1.f};

	std::string name = "undefined";

	glm::mat4 model_matrix;
	void calculateModelMatrix();
};