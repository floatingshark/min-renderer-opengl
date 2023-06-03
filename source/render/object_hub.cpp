#include "render/object_hub.hpp"

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ui/glfw_display.hpp"

ObjectHub::ObjectHub()
{
	initialize();
}

ObjectHub::~ObjectHub() {}

void ObjectHub::initialize()
{
	objects.clear();

	// 初期キューブを配置
	std::shared_ptr<RenderObject> init_cube_object = std::make_shared<RenderObject>();
	init_cube_object->setName("default cube");
	objects.push_back(init_cube_object);
}

void ObjectHub::update()
{
	// 管理しているオブジェクトを更新
	for (std::shared_ptr<RenderObject> &object : objects)
	{
		// view matrix
		if (glfw_display != nullptr)
		{
			object->getShader()->setViewMatrix(glfw_display->getViewMatrix());
			object->getShader()->setCameraPosition(glfw_display->getCameraPosition());
		}
		// light setting
		std::vector<GLfloat> l_pos(std::begin(light_position), std::end(light_position));
		object->getShader()->setLightPosition(l_pos);
		std::vector<GLfloat> l_amb(std::begin(light_ambient), std::end(light_ambient));
		object->getShader()->setLightAmbient(l_amb);
		std::vector<GLfloat> l_diff(std::begin(light_diffusion), std::end(light_diffusion));
		object->getShader()->setLightDiffusion(l_diff);
		std::vector<GLfloat> l_spec(std::begin(light_specular), std::end(light_specular));
		object->getShader()->setLightSpecular(l_spec);

		object->update();
	}
}