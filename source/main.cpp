#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <cmath>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "render/render_scene.hpp"
#include "render/render_shape.hpp"
#include "render/render_utility.hpp"
#include "shading/shading_blinn_phong.hpp"
#include "ui/glfw_display.hpp"
#include "ui/imgui_display.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int main()
{
    std::cout << "[LOG]BEGIN RENDERER" << std::endl;

    // glfw 設定
    const char *glfw_window_title = "OpenGL Renderer";
    constexpr int glfw_width = 640;
    constexpr int glfw_height = 480;
    std::shared_ptr<GLFWdisplay> glfw_display = std::make_shared<GLFWdisplay>(glfw_width, glfw_height, glfw_window_title);

    // opengl 初期化
    RenderUtility::initializeOpenGL();

    // scene 生成
    std::shared_ptr<RenderScene> scene = std::make_shared<RenderScene>();
    scene->setGlfwHandler(glfw_display);

    // ImGui 設定
    const char *glsl_version = "#version 430";
    std::unique_ptr<ImGuiDisplay> imgui_display = std::make_unique<ImGuiDisplay>(glfw_display->getGLFWWindow(), glsl_version);
    imgui_display->setDisplayHundle(glfw_display);
    imgui_display->setSceneHundle(scene);

    while (glfw_display && !glfw_display->getShouldWindowClose())
    {
        // glfw 更新
        glfw_display->update();

        // バッファーをクリア
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // シーン更新
        scene->update();

        // ImGui 描画
        imgui_display->draw();

        // glfw 描画
        glfw_display->swapBuffers();
    }

    std::cout << "[LOG]FINISH PROGRAM" << std::endl;

    return 0;
}
