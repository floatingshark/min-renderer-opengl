#pragma once
#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm/fwd.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum class DrawType
{
	Points,
	Lines,
	Triangles
};
enum class TextureType
{
	Surface,
	Normal
};
/**
 *  shader
 */
class ShadingBlinnPhong
{
public:
	ShadingBlinnPhong();
	~ShadingBlinnPhong(){};
	ShadingBlinnPhong(ShadingBlinnPhong const &) = delete;
	ShadingBlinnPhong &operator=(ShadingBlinnPhong const &) = delete;

	void update();
	void draw() const;

	void drawPoints() const;
	void drawLines() const;
	void drawTriangles() const;

	inline void setShapeVertexPosition(std::vector<GLfloat> new_position) { vertex_position = new_position; };
	inline void setShapeVertexNormal(std::vector<GLfloat> new_normal) { vertex_normal = new_normal; };
	inline void setShapeVertexIndex(std::vector<GLuint> new_index) { vertex_index = new_index; };
	inline void setShapeVertexUv(std::vector<GLfloat> new_uv) { vertex_uv = new_uv; };
	inline void setShapeVertexTangent(std::vector<GLfloat> new_tangent) { vertex_tangent = new_tangent; };
	inline void setModelMatrix(glm::mat4 new_matrix) { model_matrix = new_matrix; };
	inline void setViewMatrix(glm::mat4 new_matrix) { view_matrix = new_matrix; };

	inline std::vector<GLfloat> getCameraPosition() {return camera_position;};
	inline void setCameraPosition(std::vector<GLfloat> new_position) {camera_position = new_position;};

	inline std::vector<GLfloat> getLightPosition() { return light_position; };
	inline void setLightPosition(std::vector<GLfloat> &new_position) { light_position = new_position; };
	inline std::vector<GLfloat> getLightAmbient() { return light_ambient; };
	inline void setLightAmbient(std::vector<GLfloat> &new_ambient) { light_ambient = new_ambient; };
	inline std::vector<GLfloat> getLightDiffusion() { return light_diffusion; };
	inline void setLightDiffusion(std::vector<GLfloat> &new_diffusion) { light_diffusion = new_diffusion; };
	inline std::vector<GLfloat> getLightSpecular() { return light_specular; };
	inline void setLightSpecular(std::vector<GLfloat> &new_specular) { light_specular = new_specular; };

	inline std::vector<GLfloat> getMaterialAmbient() { return material_ambient; };
	inline void setMaterialAmbient(std::vector<GLfloat> new_ambient) { material_ambient = new_ambient; };
	inline std::vector<GLfloat> getMaterialDiffusion() { return material_diffusion; };
	inline void setMaterialDiffusion(std::vector<GLfloat> new_diffusion) { material_diffusion = new_diffusion; };
	inline std::vector<GLfloat> getMaterialSpecular() { return material_specular; };
	inline void setMaterialSpecular(std::vector<GLfloat> new_specular) { material_specular = new_specular; };
	inline GLfloat getMaterialShiness() { return material_shiness; };
	inline void setMaterialShiness(GLfloat new_shiness) { material_shiness = new_shiness; };

	inline DrawType getDrawType() const { return draw_type; };
	inline void setDrawType(const DrawType new_type) { draw_type = new_type; };

	void setBaseColor(TextureType tex_type, const std::vector<unsigned char> base_color);
	void setTexture(TextureType tex_type, const char *texture_name);
	void removeTexture(TextureType tex_type);

protected:
	// shader
	GLuint shader_program;
	const char *vert_shader_name = "shader/blinn_phong_vert.glsl";
	const char *frag_shader_name = "shader/blinn_phong_frag.glsl";

	// attribute object
	GLuint vertex_array_object;
	GLuint vertex_buffer_object;
	GLuint index_element_buffer_object;
	// uniform location
	GLint model_matrix_location;
	GLint view_matrix_location;
	GLint projection_matrix_location;
	GLint normal_matrix_location;
	GLint camera_position_location;
	GLint light_position_location;
	GLint light_ambient_location;
	GLint light_diffusion_location;
	GLint light_specular_location;
	GLint material_ambient_location;
	GLint material_diffusion_location;
	GLint material_specular_location;
	GLint material_shiness_location;
	GLuint texture_location;
	GLuint normal_texture_location;

	// attribute variables
	std::vector<GLfloat> vertex_position;
	std::vector<GLfloat> vertex_normal;
	std::vector<GLuint> vertex_index;
	std::vector<GLfloat> vertex_uv;
	std::vector<GLfloat> vertex_tangent;
	// uniform variables
	glm::mat4 model_matrix;
	glm::mat4 view_matrix;
	glm::mat4 projection_matrix;
	glm::mat3 rotate_normal_matrix;
	std::vector<GLfloat> camera_position;
	std::vector<GLfloat> light_position;
	std::vector<GLfloat> light_ambient;
	std::vector<GLfloat> light_diffusion;
	std::vector<GLfloat> light_specular;
	std::vector<GLfloat> material_ambient;
	std::vector<GLfloat> material_diffusion;
	std::vector<GLfloat> material_specular;
	GLfloat material_shiness;
	GLuint texture_id;
	GLuint normal_texture_id;

	DrawType draw_type = DrawType::Triangles;

	void setShaderProgram(const char *vert_shader_name, const char *frag_shader_name);
	void calculateAttribute();
	void assignAttribute();
	void initializeUniformVariables();
	void initializeUniformLocations();
	void assignUniform();

	void bindContext() const;
	void errorCheck() const;
};