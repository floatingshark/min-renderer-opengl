#pragma once
#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/fwd.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "shading/shading_base.hpp"

class ShadingPBR : public ShadingBase
{
public:
	ShadingPBR();
	~ShadingPBR(){};
	ShadingPBR(ShadingPBR const &) = delete;
	ShadingPBR &operator=(ShadingPBR const &) = delete;

	virtual ShaderType getShaderType() override { return ShaderType::PBR; };
	virtual void update() override;

protected:
	const char *vert_shader_name = "shader/pbr.vert";
	const char *frag_shader_name = "shader/pbr.frag";

	void calculateAttribute() override;
	void assignAttributeVariables() override;
	void initializeUniformVariables() override;
	void initializeUniformLocations() override;
	void assignUniformVariables() override;
};