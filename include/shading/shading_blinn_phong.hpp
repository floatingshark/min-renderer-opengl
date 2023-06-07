#pragma once
#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/fwd.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "shading/shading_base.hpp"

class ShadingBlinnPhong : public ShadingBase
{
public:
	ShadingBlinnPhong();
	~ShadingBlinnPhong(){};
	ShadingBlinnPhong(ShadingBlinnPhong const &) = delete;
	ShadingBlinnPhong &operator=(ShadingBlinnPhong const &) = delete;

	void update() override;

	inline std::vector<GLfloat> getMaterialAmbient() { return material_ambient; };
	inline void setMaterialAmbient(std::vector<GLfloat> new_ambient) { material_ambient = new_ambient; };
	inline std::vector<GLfloat> getMaterialDiffusion() { return material_diffusion; };
	inline void setMaterialDiffusion(std::vector<GLfloat> new_diffusion) { material_diffusion = new_diffusion; };
	inline std::vector<GLfloat> getMaterialSpecular() { return material_specular; };
	inline void setMaterialSpecular(std::vector<GLfloat> new_specular) { material_specular = new_specular; };
	inline GLfloat getMaterialShiness() { return material_shiness; };
	inline void setMaterialShiness(GLfloat new_shiness) { material_shiness = new_shiness; };
protected:
	// shader
	const char *vert_shader_name = "shader/blinn_phong_vert.glsl";
	const char *frag_shader_name = "shader/blinn_phong_frag.glsl";

	// additional uniform location
	GLint material_ambient_location;
	GLint material_diffusion_location;
	GLint material_specular_location;
	GLint material_shiness_location;

	// additional uniform variables
	std::vector<GLfloat> material_ambient;
	std::vector<GLfloat> material_diffusion;
	std::vector<GLfloat> material_specular;
	GLfloat material_shiness;

	void calculateAttribute() override;
	void assignAttributeVariables() override;
	void initializeUniformVariables() override;
	void initializeUniformLocations() override;
	void assignUniformVariables() override;
};