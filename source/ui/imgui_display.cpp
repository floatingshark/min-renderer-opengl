#include "ui/imgui_display.hpp"
#include "ui/glfw_display.hpp"
#include "render/object_hub.hpp"
#include "render/render_utility.hpp"

#include <string>
#include <filesystem>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

ImGuiDisplay::ImGuiDisplay(GLFWwindow *glfw_window, const char *glsl_version) : IO(nullptr)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	IO = &ImGui::GetIO();
	(void)IO;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(glfw_window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

ImGuiDisplay::~ImGuiDisplay()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiDisplay::draw()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	bool show_x = false;
	const ImGuiWindowFlags main_window_flags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar/*| ImGuiWindowFlags_MenuBar*/;
	ImGui::Begin("Config Window", &show_x, main_window_flags);
	ImGui::SeparatorText("Config List");
	ImGui::Text("OPENGL RENDERER");
	ImGui::Spacing();

	if (object_hub != nullptr && glfw_display != nullptr)
	{
		drawAbout();
		drawRendering();
		// drawCamera();
		drawLight();
	}
	else
	{
		ImGui::Text("AN ERROR OCCURED");
	}

	ImGui::End();

	const ImGuiWindowFlags info_window_flags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize /* | ImGuiWindowFlags_NoMove*/;
	ImGui::Begin("Info Window", &show_x, info_window_flags);
	if (object_hub != nullptr && glfw_display != nullptr)
	{
		drawInfomationWindow();
	}
	else
	{
		if (object_hub == nullptr)
		{
			ImGui::Text("object render error");
		}
		if (glfw_display == nullptr)
		{
			ImGui::Text("glfw error");
		}
	}
	ImGui::End();

	const ImGuiWindowFlags object_window_flags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar /*| ImGuiWindowFlags_NoResize /* | ImGuiWindowFlags_NoMove*/;
	ImGui::Begin("Object Window", &show_x, object_window_flags);
	if (object_hub != nullptr && glfw_display != nullptr)
	{
		drawObjectWindow();
	}
	ImGui::End();

	// demoを表示
	// ImGui::ShowDemoWindow();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (IO->WantCaptureMouse)
	{
		glfw_display->setEnableUpdate(false);
	}
	else
	{
		glfw_display->setEnableUpdate(true);
	}
}

void ImGuiDisplay::drawAbout()
{
	if (ImGui::TreeNode("About"))
	{
		ImGui::BulletText("This is a rendering application by opengl.");
		ImGui::BulletText("Github : samenachan");
		ImGui::Separator();
		ImGui::TreePop();
	}
}

void ImGuiDisplay::drawRendering()
{
	if (ImGui::TreeNode("Rendering"))
	{
		ImGui::SeparatorText("General");

		ImGui::Text("bg");
		ImGui::SameLine();
		static float background_color[4];
		if (ImGui::DragFloat4("##bg rgba", background_color, 0.01f, 0.f, 1.f))
		{
			RenderUtility::setBackgroundColor(background_color);
		}
		static bool enableCullFace = true;
		if (ImGui::Checkbox("GL_CULL_FACE", &enableCullFace))
		{
			RenderUtility::enableCullFace(enableCullFace);
		}
		static bool enableDepthTest = true;
		if (ImGui::Checkbox("GL_DEPTH_TEST", &enableDepthTest))
		{
			RenderUtility::enableDepthTest(enableDepthTest);
		}
		ImGui::TreePop();
	}
}

void ImGuiDisplay::drawCamera()
{
	if (ImGui::TreeNode("Camera Settings"))
	{
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode("Camera Location"))
		{
			ImGui::DragFloat3("##xyz", glfw_display->camera_position, 0.1f, -100.f, 100.f);
			ImGui::TreePop();
		}
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode("##Camera Lookat"))
		{
			ImGui::DragFloat3("xyz", glfw_display->camera_lookat, 0.1f, -100.f, 100.f);
			ImGui::TreePop();
		}
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode("##Camera Up"))
		{
			ImGui::DragFloat3("xyz", glfw_display->camera_up, 0.1f, -100.f, 100.f);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

void ImGuiDisplay::drawLight()
{
	if (ImGui::TreeNode("Light"))
	{
		ImGui::Text("Pos");
		ImGui::SameLine();
		ImGui::DragFloat4("##light_position", object_hub->light_position, 0.1f, -100.f, 100.f, "%.2f");
		ImGui::Text("Amb");
		ImGui::SameLine();
		ImGui::DragFloat3("##light_ambient", object_hub->light_ambient, 0.1f, 0.f, 1.f, "%.2f");
		ImGui::Text("Dif");
		ImGui::SameLine();
		ImGui::DragFloat3("##light_diffusion", object_hub->light_diffusion, 0.1f, 0.f, 1.f, "%.2f");
		ImGui::Text("Spe");
		ImGui::SameLine();
		ImGui::DragFloat3("##light_specular", object_hub->light_specular, 0.1f, -0.f, 1.f, "%.2f");
		ImGui::TreePop();
	}
}

void ImGuiDisplay::drawInfomationWindow()
{
	ImGui::SeparatorText("Info");
	ImGui::Text("fps : %.1f", getIO()->Framerate);
	ImGui::SeparatorText("Camera");
	ImGui::Text("x : %.2f", glfw_display->getCameraPosition()[0]);
	ImGui::Text("y : %.2f", glfw_display->getCameraPosition()[1]);
	ImGui::Text("z : %.2f", glfw_display->getCameraPosition()[2]);
}

void ImGuiDisplay::drawObjectWindow()
{
	ImGui::SeparatorText("Object List");
	static int select_id = 0;
	static ImGuiTableFlags object_table_flags =
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingFixedFit;
	if (ImGui::BeginTable("Object Table", 2, object_table_flags, ImVec2(0.0f, 10.f * 7.f), 0.0f))
	{
		ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupScrollFreeze(0, 1);
		ImGui::TableHeadersRow();

		int index = 0;
		char id_label[32];
		for (std::shared_ptr<RenderObject> object : object_hub->getObjects())
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			sprintf(id_label, "%d", index);
			if (ImGui::Selectable(id_label, false, ImGuiSelectableFlags_SpanAllColumns, ImVec2(0.0f, 0.f)))
			{
				select_id = index;
			}
			ImGui::TableNextColumn();
			ImGui::Text(object->getName().c_str());
			index++;
		}
		ImGui::EndTable();
	}

	ImGui::SeparatorText("Details");
	if (select_id < static_cast<int>(object_hub->getObjects().size()))
	{
		std::shared_ptr<RenderObject> object = object_hub->getObjects().at(select_id);
		ImGui::BulletText(object->getName().c_str());

		if (ImGui::TreeNode("Transform"))
		{
			ImGui::Text("Pos");
			ImGui::SameLine();
			static std::vector<float> object_position = object->getPosition();
			if (ImGui::DragFloat3("##object_position", object_position.data(), 0.1f, -100.f, 100.f))
			{
				object->setPosition(object_position);
			}
			ImGui::Text("Rot");
			ImGui::SameLine();
			static std::vector<float> object_rotation = object->getRotation();
			if (ImGui::DragFloat3("##object_rotation", object_rotation.data(), 0.1f, -100.f, 100.f))
			{
				object->setRotation(object_rotation);
			}
			ImGui::Text("Sca");
			ImGui::SameLine();
			static std::vector<float> object_scale = object->getScale();
			if (ImGui::DragFloat3("##object_scale", object_scale.data(), 0.1f, -100.f, 100.f))
			{
				object->setScale(object_scale);
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Material"))
		{
			ImGui::Text("Amb");
			ImGui::SameLine();
			static std::vector<GLfloat> material_ambient = object->getShader()->getMaterialAmbient();
			if (ImGui::DragFloat3("##material_ambient", material_ambient.data(), 0.01f, 0.f, 1.f))
			{
				object->getShader()->setMaterialAmbient(material_ambient);
			}
			ImGui::Text("Dif");
			ImGui::SameLine();
			static std::vector<GLfloat> material_diffusion = object->getShader()->getMaterialDiffusion();
			if (ImGui::DragFloat3("##material_diffusion", material_diffusion.data(), 0.01f, 0.f, 1.f))
			{
				object->getShader()->setMaterialDiffusion(material_diffusion);
			}
			ImGui::Text("Spe");
			ImGui::SameLine();
			static std::vector<GLfloat> material_specular = object->getShader()->getMaterialDiffusion();
			if (ImGui::DragFloat3("##material_specular", material_specular.data(), 0.01f, 0.f, 1.f))
			{
				object->getShader()->setMaterialSpecular(material_specular);
			}
			ImGui::Text("Shi");
			ImGui::SameLine();
			static GLfloat material_shiness = object->getShader()->getMaterialShiness();
			if (ImGui::DragFloat("##material_shiness", &material_shiness, 0.1f, 0.f, 100.f))
			{
				object->getShader()->setMaterialShiness(material_shiness);
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Texture"))
		{
			ImGui::SeparatorText("Texture");
			static ImGuiTableFlags texture_table_flags =
				ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingFixedFit;
			if (ImGui::BeginTable("##Object_Texture", 1, texture_table_flags, ImVec2(0.0f, 5.f * 15.f), 0.0f))
			{
				ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableSetupScrollFreeze(0, 1);
				ImGui::TableHeadersRow();

				const std::string texture_path = "texture/";

				for (const auto &file : std::filesystem::directory_iterator(texture_path))
				{
					const std::string texture_diff_name = file.path().string();
					const std::string::size_type diff_find = texture_diff_name.find("diff");
					if (diff_find != std::string::npos)
					{
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						if (ImGui::Selectable(texture_diff_name.c_str(), false, ImGuiSelectableFlags_SpanAllColumns, ImVec2(0.0f, 0.f)))
						{
							const std::string texture_name = texture_diff_name.substr(0, texture_diff_name.size() - 12);
							const std::string texture_nor_name = texture_name + "_nor_gl_1k.png";
							object->getShader()->setTexture(TextureType::Surface, texture_diff_name.c_str());
							object->getShader()->setTexture(TextureType::Normal, texture_nor_name.c_str());
						}
					}
				}

				ImGui::EndTable();
			}

			ImGui::SeparatorText("BaseColor");
			static std::vector<int> object_base_color = {255, 255, 255};
			if (ImGui::DragInt3("##Object_Base_Color", object_base_color.data(), 1, 0, 255))
			{
				std::vector<unsigned char> unsigned_color = {static_cast<unsigned char>(object_base_color[0]), static_cast<unsigned char>(object_base_color[1]), static_cast<unsigned char>(object_base_color[2])};
				object->getShader()->setBaseColor(TextureType::Surface, unsigned_color);
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Shape"))
		{
			ImGui::Text("Shape Type");
			const char *shape_type_enums[] = {"Plane", "Cube", "Sphere"};
			static int shape_type_int = static_cast<int>(object->getShape()->getShapeType());
			if (ImGui::Combo("##Shape Types", &shape_type_int, shape_type_enums, IM_ARRAYSIZE(shape_type_enums)))
			{
				const ShapeType shape_type = static_cast<ShapeType>(shape_type_int);
				object->getShape()->setShapeType(shape_type);
				object->refreshShape();
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Rendering"))
		{
			ImGui::Text("Draw Type");
			const char *draw_type_enums[] = {"Points", "Lines", "Triangles"};
			static int draw_type_int = static_cast<int>(object->getShader()->getDrawType());
			if (ImGui::Combo("##Draw Types", &draw_type_int, draw_type_enums, IM_ARRAYSIZE(draw_type_enums)))
			{
				const DrawType draw_type = static_cast<DrawType>(draw_type_int);
				object->getShader()->setDrawType(draw_type);
			}
			ImGui::TreePop();
		}
	}
	else
	{
		ImGui::Text("An Error Occured or No Object.");
	}
}