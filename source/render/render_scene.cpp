#include "render/render_scene.hpp"

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ui/glfw_display.hpp"

RenderScene::RenderScene()
{
	initialize();
}

RenderScene::~RenderScene() {}

void RenderScene::initialize()
{
	objects.clear();

	// 初期キューブを配置
	std::shared_ptr<RenderObject> init_cube_object = std::make_shared<RenderObject>();
	init_cube_object->setName("default cube");
	objects.push_back(init_cube_object);
}

void RenderScene::update()
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

		object->update();
	}
}