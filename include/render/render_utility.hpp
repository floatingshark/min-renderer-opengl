#pragma once
#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

class RenderUtility
{
public:
	static void initializeOpenGL();
	static void setBackgroundColor(const float color[4]);
	static void enableCullFace(const bool enable);
	static void enableDepthTest(const bool enable);

	static GLuint loadShaderProgram(const char *vert, const char *frag);
	static std::vector<unsigned char> loadTexturePng(const char *path);
	static void setActiveTexture(GLuint number);
	static GLuint generateTexture(const std::vector<unsigned char> data);
	static void unbindTexture(GLuint tex_id);

	template <class T>
	static std::vector<T> flattenVector(const std::vector<std::vector<T>> &in_vector)
	{
		std::vector<T> flat_vector;
		for (int i = 0; i < static_cast<int>(in_vector.size()); i++)
		{
			for (int j = 0; j < static_cast<int>(in_vector.at(0).size()); j++)
			{
				flat_vector.push_back(in_vector[i][j]);
			}
		}
		return flat_vector;
	}

private:
	static GLboolean printShaderInfoLog(GLuint shader, const char *str);
	static GLboolean printProgramInfoLog(GLuint program);
	static GLuint createProgram(const char *vsrc, const char *fsrc);
	static bool readShaderSource(const char *name, std::vector<GLchar> &buffer);

	static void APIENTRY errorCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
	{
		(void)source;
		(void)type;
		(void)id;
		(void)severity;
		(void)length;
		(void)userParam;

		if (severity == GL_DEBUG_SEVERITY_HIGH)
		{
			std::cerr << "[Error]" << message << std::endl;
		}
		else if (severity == GL_DEBUG_LOGGED_MESSAGES)
		{
			std::cout << "[Log]" << message << std::endl;
		}
		else if (severity == GL_DEBUG_SEVERITY_MEDIUM)
		{
			std::cout << "[Warn]" << message << std::endl;
		}
		else if (severity == GL_DEBUG_SEVERITY_LOW)
		{
			std::cout << "[Notice]" << message << std::endl;
		}
		else if (severity == GL_DEBUG_OUTPUT)
		{
			std::cout << "[Output]" << message << std::endl;
		}
		else
		{
			// std::cout << message << std::endl;
		}
	}
};