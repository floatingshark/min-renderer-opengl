#include "render/render_utility.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <cmath>
#include <GL/glew.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

void RenderUtility::initializeOpenGL()
{
	// 背景色
	glClearColor(0.05f, 0.05f, 0.05f, 0.0f);

	// カリング設定
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	// 深度テスト設定
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	// エラーコールバック設定
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(errorCallbackFunction, nullptr);
}

void RenderUtility::setClearColor(const float color[4])
{
	glClearColor(color[0], color[1], color[2], color[3]);
}

void RenderUtility::enableCullFace(const bool enable)
{
	if (enable)
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}

void RenderUtility::enableDepthTest(const bool enable)
{
	if (enable)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
}

GLuint RenderUtility::loadShaderProgram(const char *vert, const char *frag)
{
	std::vector<GLchar> vsrc;
	const bool vstat(readShaderSource(vert, vsrc));
	std::vector<GLchar> fsrc;
	const bool fstat(readShaderSource(frag, fsrc));
	return vstat && fstat ? createProgram(vsrc.data(), fsrc.data()) : 0;
}

std::vector<unsigned char> RenderUtility::loadTexturePng(const char *path)
{
	cv::Mat image = cv::imread(path);

	std::vector<unsigned char> image_data(image.cols * image.rows * 3);
	for (int x = 0; x < image.cols; x++)
	{
		for (int y = 0; y < image.rows; y++)
		{
			image_data[(x * image.rows + y) * 3 + 0] = image.at<cv::Vec3b>(y, x)[2];
			image_data[(x * image.rows + y) * 3 + 1] = image.at<cv::Vec3b>(y, x)[1];
			image_data[(x * image.rows + y) * 3 + 2] = image.at<cv::Vec3b>(y, x)[0];
		}
	}

	return image_data;
}

void RenderUtility::setActiveTexture(GLuint number)
{
	switch (number)
	{
	case 0:
		glActiveTexture(GL_TEXTURE0);
		break;
	case 1:
		glActiveTexture(GL_TEXTURE1);
		break;
	case 2:
		glActiveTexture(GL_TEXTURE2);
		break;
	default:
		break;
	}
}

GLuint RenderUtility::generateTexture(std::vector<unsigned char> data)
{
	GLuint tex_id = 0;
	glGenTextures(1, &tex_id);

	setActiveTexture(tex_id - 1);
	glBindTexture(GL_TEXTURE_2D, tex_id);

	const GLint tex_size = std::sqrt(data.size() / 3);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_size, tex_size, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return tex_id;
}

void RenderUtility::unbindTexture(GLuint tex_id)
{
	if (tex_id < 1)
	{
		return;
	}

	setActiveTexture(tex_id - 1);
	glDeleteTextures(1, &tex_id);
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool RenderUtility::readShaderSource(const char *name, std::vector<GLchar> &buffer)
{
	if (name == NULL)
		return false;

	std::ifstream file(name, std::ios::binary);
	if (file.fail())
	{
		std::cerr << "Error: Can't open source file: " << name << std::endl;
		return false;
	}

	file.seekg(0L, std::ios::end);
	GLsizei length = static_cast<GLsizei>(file.tellg());
	buffer.resize(length + 1);
	file.seekg(0L, std::ios::beg);
	file.read(buffer.data(), length);
	buffer[length] = '\0';
	if (file.fail())
	{
		std::cerr << "Error: Could not read souce file: " << name << std::endl;
		file.close();
		return false;
	}
	file.close();
	return true;
}

GLuint RenderUtility::createProgram(const char *vsrc, const char *fsrc)
{
	const GLuint program(glCreateProgram());
	if (vsrc != NULL)
	{
		const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
		glShaderSource(vobj, 1, &vsrc, NULL);
		glCompileShader(vobj);
		if (printShaderInfoLog(vobj, "vertex shader"))
			glAttachShader(program, vobj);
		glDeleteShader(vobj);
	}

	if (fsrc != NULL)
	{
		const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
		glShaderSource(fobj, 1, &fsrc, NULL);
		glCompileShader(fobj);
		if (printShaderInfoLog(fobj, "fragment shader"))
			glAttachShader(program, fobj);
		glDeleteShader(fobj);
	}

	glBindAttribLocation(program, 0, "position");
	glBindAttribLocation(program, 1, "normal");
	glBindAttribLocation(program, 2, "uv");
	glBindAttribLocation(program, 3, "tangent");
	glBindFragDataLocation(program, 0, "fragment");
	glLinkProgram(program);

	if (printProgramInfoLog(program))
	{
		return program;
	}
	else
	{
		glDeleteProgram(program);
		return 0;
	}
}

GLboolean RenderUtility::printShaderInfoLog(GLuint shader, const char *str)
{
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
		std::cerr << "Compile Error in " << str << std::endl;

	GLsizei bufSize;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);
	if (bufSize > 1)
	{
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}
	return static_cast<GLboolean>(status);
}

GLboolean RenderUtility::printProgramInfoLog(GLuint program)
{
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		std::cerr << "[LOG]LINK SHADER ERROR." << std::endl;
	}
	else if (status == GL_TRUE)
	{
		std::cout << "[LOG]LINK SHADER PROGRAM" << std::endl;
	}

	GLsizei bufSize;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);
	if (bufSize > 1)
	{
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}
	return static_cast<GLboolean>(status);
}
