#include "shading/shading_blinn_phong.hpp"
#include "render/render_utility.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/matrix_operation.hpp"

ShadingBlinnPhong::ShadingBlinnPhong()
{
    ShadingBase::setShaderProgram(vert_shader_name, frag_shader_name);

    initializeUniformVariables();
    initializeUniformLocations();
}

void ShadingBlinnPhong::update()
{
    ShadingBase::update();

    // shaderを使用
    glUseProgram(shader_program);

    // attribute変数を適用
    calculateAttribute();
    assignAttributeVariables();

    // uniform変数を適用
    assignUniformVariables();

    // vaoをバインド
    bindContext();
}

void ShadingBlinnPhong::calculateAttribute()
{
    ShadingBase::calculateAttribute();
}

void ShadingBlinnPhong::assignAttributeVariables()
{
    ShadingBase::assignAttributeVariables();

    // 各頂点データを結合
    std::vector<GLfloat> vertex_context;
    vertex_context.insert(vertex_context.end(), vertex_position.begin(), vertex_position.end());
    vertex_context.insert(vertex_context.end(), vertex_normal.begin(), vertex_normal.end());
    vertex_context.insert(vertex_context.end(), vertex_uv.begin(), vertex_uv.end());
    vertex_context.insert(vertex_context.end(), vertex_tangent.begin(), vertex_tangent.end());

    // vertex buffer object 生成
    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);

    // vertex buffer object 生成して頂点データを転送
    glGenBuffers(1, &vertex_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER, vertex_context.size() * sizeof(GLfloat), &vertex_context[0], GL_STATIC_DRAW);

    // 各頂点データをシェーダのattribute変数に対応付ける
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(vertex_position.size() * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)((vertex_position.size() + vertex_normal.size()) * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)((vertex_position.size() + vertex_normal.size() + vertex_uv.size()) * sizeof(GLfloat)));
    glEnableVertexAttribArray(3);

    // indexをelement bufferに格納
    glGenBuffers(1, &index_element_buffer_object);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_element_buffer_object);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertex_index.size() * sizeof(GLuint), &vertex_index[0], GL_STATIC_DRAW);
}

void ShadingBlinnPhong::initializeUniformVariables()
{
    ShadingBase::initializeUniformVariables();

    setMaterialAmbient({1.f, 1.f, 1.f});
    setMaterialDiffusion({0.3f, 0.3f, 0.3f});
    setMaterialSpecular({1.f, 1.f, 1.f});
    setMaterialShiness(10.f);
}

void ShadingBlinnPhong::initializeUniformLocations()
{
    ShadingBase::initializeUniformLocations();

    material_ambient_location = glGetUniformLocation(shader_program, "u_f3_material_ambient");
    material_diffusion_location = glGetUniformLocation(shader_program, "u_f3_material_diffusion");
    material_specular_location = glGetUniformLocation(shader_program, "u_f3_material_specular");
    material_shiness_location = glGetUniformLocation(shader_program, "u_f_material_shiness");
}

void ShadingBlinnPhong::assignUniformVariables()
{
    ShadingBase::assignUniformVariables();

    glUniform3fv(material_ambient_location, 1, material_ambient.data());
    glUniform3fv(material_diffusion_location, 1, material_diffusion.data());
    glUniform3fv(material_specular_location, 1, material_specular.data());
    glUniform1f(material_shiness_location, material_shiness);
}
