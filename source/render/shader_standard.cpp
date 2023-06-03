#include "render/shader_standard.hpp"
#include "render/render_utility.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/matrix_operation.hpp"

ShaderStandard::ShaderStandard()
{
    setShaderProgram(vert_shader_name, frag_shader_name);

    initializeUniformVariables();
    initializeUniformLocations();
}

void ShaderStandard::setShaderProgram(const char *vert_shader_name, const char *frag_shader_name)
{
    shader_program = RenderUtility::loadShaderProgram(vert_shader_name, frag_shader_name);
}

void ShaderStandard::update()
{
    // shaderを使用
    glUseProgram(shader_program);

    // attribute変数を適用
    calculateAttribute();
    assignAttribute();

    // uniform変数を適用
    assignUniform();

    // vaoをバインド
    bindContext();
}

void ShaderStandard::draw() const
{
    switch (draw_type)
    {
    case DrawType::Points:
        drawPoints();
        break;
    case DrawType::Lines:
        drawLines();
        break;
    case DrawType::Triangles:
        drawTriangles();
        break;
    default:
        break;
    }
}

void ShaderStandard::calculateAttribute()
{
}

void ShaderStandard::assignAttribute()
{
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

void ShaderStandard::initializeUniformVariables()
{
    setMaterialAmbient({1.f, 1.f, 1.f});
    setMaterialDiffusion({1.f, 1.f, 1.f});
    setMaterialSpecular({1.f, 1.f, 1.f});
    setMaterialShiness(10.f);

    texture_id = 0;
    normal_texture_id = 0;
    setBaseColor(TextureType::Surface, {255, 255, 255});
    setBaseColor(TextureType::Normal, {128, 128, 255});
}

void ShaderStandard::initializeUniformLocations()
{
    model_matrix_location = glGetUniformLocation(shader_program, "u_m4_model_matrix");
    view_matrix_location = glGetUniformLocation(shader_program, "u_m4_view_matrix");
    projection_matrix_location = glGetUniformLocation(shader_program, "u_m4_projection_matrix");
    normal_matrix_location = glGetUniformLocation(shader_program, "u_m4_rotate_normal_matrix");

    camera_position_location = glGetUniformLocation(shader_program, "u_f3_camera_position");
    light_position_location = glGetUniformLocation(shader_program, "u_f4_light_position");
    light_ambient_location = glGetUniformLocation(shader_program, "u_f3_light_ambient");
    light_diffusion_location = glGetUniformLocation(shader_program, "u_f3_light_diffusion");
    light_specular_location = glGetUniformLocation(shader_program, "u_f3_light_specular");
    material_ambient_location = glGetUniformLocation(shader_program, "u_f3_material_ambient");
    material_diffusion_location = glGetUniformLocation(shader_program, "u_f3_material_diffusion");
    material_specular_location = glGetUniformLocation(shader_program, "u_f3_material_specular");
    material_shiness_location = glGetUniformLocation(shader_program, "u_f_material_shiness");
    texture_location = glGetUniformLocation(shader_program, "u_i_texture");
    normal_texture_location = glGetUniformLocation(shader_program, "u_i_normal_texture");
}

void ShaderStandard::assignUniform()
{
    projection_matrix = glm::perspective(glm::radians(60.f), 4.f / 3.f, 0.1f, 100.f);
    const glm::mat3 m4_matrix = glm::mat3(model_matrix);
    rotate_normal_matrix = glm::transpose(glm::adjugate(m4_matrix));

    glUniformMatrix4fv(model_matrix_location, 1, GL_FALSE, &model_matrix[0][0]);
    glUniformMatrix4fv(view_matrix_location, 1, GL_FALSE, &view_matrix[0][0]);
    glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE, &projection_matrix[0][0]);
    glUniformMatrix3fv(normal_matrix_location, 1, GL_FALSE, &rotate_normal_matrix[0][0]);

    glUniform3fv(camera_position_location, 1, camera_position.data());
    glUniform4fv(light_position_location, 1, light_position.data());
    glUniform3fv(light_ambient_location, 1, light_ambient.data());
    glUniform3fv(light_diffusion_location, 1, light_diffusion.data());
    glUniform3fv(light_specular_location, 1, light_specular.data());

    glUniform3fv(material_ambient_location, 1, material_ambient.data());
    glUniform3fv(material_diffusion_location, 1, material_diffusion.data());
    glUniform3fv(material_specular_location, 1, material_specular.data());
    glUniform1f(material_shiness_location, material_shiness);

    glUniform1i(texture_location, 0);
    glUniform1i(normal_texture_location, 1);
}

void ShaderStandard::bindContext() const
{
    // 使用するvaoを指定する
    glBindVertexArray(vertex_array_object);
}

void ShaderStandard::errorCheck() const
{
    // error checker
    auto err = glGetError();
    if (err != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error: " << err << std::endl;
    }
}

void ShaderStandard::drawPoints() const
{
    glDrawElements(GL_POINTS, vertex_index.size(), GL_UNSIGNED_INT, 0);
}

void ShaderStandard::drawLines() const
{
    glDrawElements(GL_LINE_STRIP, vertex_index.size(), GL_UNSIGNED_INT, 0);
}

void ShaderStandard::drawTriangles() const
{
    glDrawElements(GL_TRIANGLES, vertex_index.size(), GL_UNSIGNED_INT, 0);
}

void ShaderStandard::setBaseColor(TextureType tex_type, const std::vector<unsigned char> base_color)
{
    removeTexture(tex_type);

    switch (tex_type)
    {
    case TextureType::Surface:
        texture_id = RenderUtility::generateTexture(base_color);
        break;
    case TextureType::Normal:
        normal_texture_id = RenderUtility::generateTexture(base_color);
        break;
    default:
        break;
    }
}

void ShaderStandard::setTexture(TextureType tex_type, const char *texture_name)
{
    removeTexture(tex_type);
    std::vector<unsigned char> texture_data = RenderUtility::loadTexturePng(texture_name);
    switch (tex_type)
    {
    case TextureType::Surface:
        texture_id = RenderUtility::generateTexture(texture_data);
        break;
    case TextureType::Normal:
        normal_texture_id = RenderUtility::generateTexture(texture_data);
        break;
    default:
        break;
    }
}

void ShaderStandard::removeTexture(TextureType tex_type)
{
    switch (tex_type)
    {
    case TextureType::Surface:
        RenderUtility::unbindTexture(texture_id);
        break;
    case TextureType::Normal:
        RenderUtility::unbindTexture(normal_texture_id);
        break;
    default:
        break;
    }
}