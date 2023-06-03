#include "ui/glfw_display.hpp"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

GLFWdisplay::GLFWdisplay(int width, int height, const char *title) : window(nullptr), zoom_scale(100.0f), cursor_location{0.0f, 0.0f}, key_status(GLFW_RELEASE)
{
	// glfw 初期化
	if (glfwInit() == GL_FALSE)
	{
		std::cerr << "Can't initialize GLFW" << std::endl;
		exit(1);
	}

	// glfw window生成
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Can't create GLFW window." << std::endl;
		exit(1);
	}

	// glfw 設定
	glfwMakeContextCurrent(window);
	atexit(glfwTerminate);
	glfwSwapInterval(1);
	glfwSetTime(0.0);

	// glew 初期化
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Can't initialize GLEW" << std::endl;
		exit(1);
	}

	// control
	window_size[0] = static_cast<GLfloat>(width);
	window_size[1] = static_cast<GLfloat>(height);

	// callback登録
	glfwSetWindowUserPointer(window, this);
	glfwSetWindowSizeCallback(window, resizeCallback);
	glfwSetCursorPosCallback(window, cursorPositionCallback);
	glfwSetScrollCallback(window, wheelCallback);
	glfwSetKeyCallback(window, keyboardCallback);
}

GLFWdisplay::~GLFWdisplay()
{
	glfwDestroyWindow(window);
}

void GLFWdisplay::update()
{
	glfwPollEvents();

	calculateViewMatrix();
}

void GLFWdisplay::swapBuffers() const
{
	glfwSwapBuffers(window);
}

bool GLFWdisplay::getShouldWindowClose() const
{
	return glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE);
}

void GLFWdisplay::resizeCallback(GLFWwindow *const window, int width, int height)
{
	int fbWidth, fbHeight;
	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
	glViewport(0, 0, fbWidth, fbHeight);

	GLFWdisplay *const instance(static_cast<GLFWdisplay *>(glfwGetWindowUserPointer(window)));

	if (instance != NULL)
	{
		instance->window_size[0] = static_cast<GLfloat>(width);
		instance->window_size[1] = static_cast<GLfloat>(height);
	}
}

void GLFWdisplay::cursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
{
	GLFWdisplay *const instance(static_cast<GLFWdisplay *>(glfwGetWindowUserPointer(window)));
	if (instance != nullptr)
	{
		if (!instance->enable_update)
		{
			return;
		}

		// カーソル位置更新
		const float prev_cursor[2] = {instance->cursor_location[0], instance->cursor_location[1]};
		instance->cursor_location[0] = static_cast<float>(xpos) * 2.0f / instance->window_size[0] - 1.0f;
		instance->cursor_location[1] = 1.0f - static_cast<float>(ypos) * 2.0f / instance->window_size[1];
		// カメラ位置の更新
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) != GLFW_RELEASE)
		{
			constexpr float move_scale = 1.f;
			const float x_move = instance->cursor_location[0] - prev_cursor[0];
			const float y_move = instance->cursor_location[1] - prev_cursor[1];
			const glm::vec3 c_pos = {instance->camera_position[0], instance->camera_position[1], instance->camera_position[2]};
			const glm::vec3 c_rel_pos = {instance->camera_position[0] - instance->camera_lookat[0], instance->camera_position[1] - instance->camera_lookat[1], instance->camera_position[2] - instance->camera_lookat[2]};
			const glm::vec3 c_pos_xy = glm::normalize(glm::vec3(c_rel_pos[0], c_rel_pos[1], 0.f));
			const float dot_xy = glm::dot(c_pos_xy, glm::vec3(1.f, 0.f, 0.f));
			const float angle_xy_sign = (c_pos_xy[0] * 0.f - c_pos_xy[1] * 1.f) >= 0.f ? -1.f : 1.f;
			const float angle_xy_rad = angle_xy_sign * glm::acos(dot_xy);
			const glm::mat4 xy_rotate_vector_matrix = glm::rotate(glm::mat4(1.f), angle_xy_rad, glm::vec3(0.f, 0.f, 1.f));
			const glm::vec4 xy_rotate_vector = xy_rotate_vector_matrix * glm::vec4(0.f, 1.f, 0.f, 1.f);
			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
			{
				const glm::mat4 x_rotate_mat = glm::rotate(glm::mat4(1.f), -x_move * move_scale, glm::vec3(0.f, 0.f, 1.f));
				const glm::mat4 y_rotate_mat = glm::rotate(glm::mat4(1.f), y_move * move_scale, glm::vec3(xy_rotate_vector));
				const glm::vec4 dest_pos = y_rotate_mat * x_rotate_mat * glm::vec4(c_pos, 1.f);
				instance->camera_position[0] = dest_pos[0];
				instance->camera_position[1] = dest_pos[1];
				instance->camera_position[2] = dest_pos[2];
			}
			else
			{
				instance->camera_position[0] += -x_move * xy_rotate_vector[0] * move_scale;
				instance->camera_lookat[0] += -x_move * xy_rotate_vector[0] * move_scale;
				instance->camera_position[1] += -x_move * xy_rotate_vector[1] * move_scale;
				instance->camera_lookat[1] += -x_move * xy_rotate_vector[1] * move_scale;
				instance->camera_position[2] += -y_move * move_scale;
				instance->camera_lookat[2] += -y_move * move_scale;
			}
		}
	}
}

void GLFWdisplay::wheelCallback(GLFWwindow *window, double /*xoffset*/, double yoffset)
{
	GLFWdisplay *const instance(static_cast<GLFWdisplay *>(glfwGetWindowUserPointer(window)));
	if (instance != nullptr)
	{
		if (!instance->enable_update)
		{
			return;
		}

		instance->zoom_scale += static_cast<GLfloat>(yoffset);
		constexpr float zoom_speed = 0.1f;
		const float zoom_vector[3] = {instance->camera_lookat[0] - instance->camera_position[0], instance->camera_lookat[1] - instance->camera_position[1], instance->camera_lookat[2] - instance->camera_position[2]};
		instance->camera_position[0] = instance->camera_position[0] + zoom_vector[0] * yoffset * zoom_speed;
		instance->camera_position[1] = instance->camera_position[1] + zoom_vector[1] * yoffset * zoom_speed;
		instance->camera_position[2] = instance->camera_position[2] + zoom_vector[2] * yoffset * zoom_speed;
	}
}

void GLFWdisplay::keyboardCallback(GLFWwindow *window, int /*key*/, int /*scancode*/, int action, int /*mods*/)
{
	GLFWdisplay *const instance(static_cast<GLFWdisplay *>(glfwGetWindowUserPointer(window)));
	if (instance != NULL)
	{
		instance->key_status = action;
	}
}

void GLFWdisplay::calculateViewMatrix()
{
	const glm::vec3 view_eye = {camera_position[0], camera_position[1], camera_position[2]};
	const glm::vec3 view_lookat = {camera_lookat[0], camera_lookat[1], camera_lookat[2]};
	const glm::vec3 view_up = {camera_up[0], camera_up[1], camera_up[2]};
	view_matrix = glm::lookAt(view_eye, view_lookat, view_up);
}
