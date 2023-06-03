#pragma once
#include <memory>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "render/object_hub.hpp"

class ImGuiDisplay
{
private:
	ImGuiIO *IO;

public:
	ImGuiDisplay(GLFWwindow *glfw_window, const char *glsl_version);
	virtual ~ImGuiDisplay();
	ImGuiDisplay(ImGuiDisplay const &) = delete;
	ImGuiDisplay &operator=(ImGuiDisplay const &) = delete;

	void draw();

	inline ImGuiIO *getIO() const { return IO; }
	inline void setDisplayHundle(std::shared_ptr<GLFWdisplay> handle) {glfw_display = handle;};
	inline void setObjectHundle(std::shared_ptr<ObjectHub> handle) { object_hub = handle; };

private:
	std::shared_ptr<GLFWdisplay> glfw_display;
	std::shared_ptr<ObjectHub> object_hub;

	void drawAbout();
	void drawRendering();
	void drawCamera();
	void drawLight();

	void drawInfomationWindow();
	void drawObjectWindow();
};