#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/fwd.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "render/render_object.hpp"
#include "render/render_shape.hpp"
#include "shader_standard.hpp"

class GLFWdisplay;

/**
 * object management
*/
class ObjectHub
{
public:
	ObjectHub();
	~ObjectHub();
	ObjectHub(ObjectHub const &) = delete;
	ObjectHub &operator=(ObjectHub const &) = delete;

	float light_position[4] = {10.f, 10.f, 10.f, 1.0f};
	float light_ambient[3] = {0.03f, 0.03f, 0.03f};
	float light_diffusion[3] = {0.3f, 0.3f, 0.3f};
	float light_specular[3] = {0.8f, 0.8f, 0.8f};

	void initialize();
	void update();
	inline std::vector<std::shared_ptr<RenderObject>> getObjects() const { return objects; };

	inline void setGlfwHandler(std::shared_ptr<GLFWdisplay> handle) { glfw_display = handle; };

protected:
	std::shared_ptr<GLFWdisplay> glfw_display;
	std::vector<std::shared_ptr<RenderObject>> objects;
};