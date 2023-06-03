#pragma once
#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

class GLFWdisplay
{
public:
	float camera_position[3] = {10.f, 5.f, 5.f};
	float camera_lookat[3] = {0.f, 0.f, 0.f};
	float camera_up[3] = {0.f, 0.f, 1.f};

protected:
	GLFWwindow *window;
	glm::mat4 view_matrix;

	GLfloat window_size[2] = {640.f, 480.f};
	GLfloat zoom_scale = 100.f;
	float cursor_location[2] = {0.f, 0.f};
	int key_status;
	bool enable_update = false;
public:
	GLFWdisplay(int width = 640, int height = 480, const char *title = "glfw window");
	virtual ~GLFWdisplay();
	GLFWdisplay(GLFWdisplay const &) = delete;
	GLFWdisplay &operator=(GLFWdisplay const &) = delete;

	inline void setEnableUpdate(bool new_enable) { enable_update = new_enable; };

	void update();
	void swapBuffers() const;
	bool getShouldWindowClose() const;

	static void resizeCallback(GLFWwindow *const window, int width, int height);
	static void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos);
	static void wheelCallback(GLFWwindow *window, double /*xoffset*/, double yoffset);
	static void keyboardCallback(GLFWwindow *window, int /*key*/, int /*scancode*/, int action, int /*mods*/);

	inline std::vector<float> getCameraPosition() { return std::vector<float>(std::begin(camera_position), std::end(camera_position)); };
	inline std::vector<float> getCameraLookat() { return std::vector<float>(std::begin(camera_lookat), std::end(camera_lookat)); };
	inline std::vector<float> getCameraUp() { return std::vector<float>(std::begin(camera_up), std::end(camera_up)); };
	inline glm::mat4 getViewMatrix() { return view_matrix; };

	inline const GLfloat *getWindowSize() const { return window_size; }
	inline GLfloat getZoomScale() const { return zoom_scale; }
	inline const float *getCursorLocation() const { return cursor_location; }
	inline GLFWwindow *getGLFWWindow() const { return window; }

protected:
	void calculateViewMatrix();
};