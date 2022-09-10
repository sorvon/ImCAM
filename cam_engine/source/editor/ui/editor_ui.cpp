#include "editor_ui.h"
#include "runtime/cam_engine.h"

#include <stb_image.h>
#include <glfw/glfw3.h>
#include <imgui.h>
#include <imgui_internal.h>

namespace ImCAM
{
    EditorUI::EditorUI()
    {

    }

    void EditorUI::initialize(WindowUIInitInfo init_info)
    {
        // std::shared_ptr<ConfigManager> config_manager = g_runtime_global_context.m_config_manager;
        // ASSERT(config_manager);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        // set ui content scale
        float x_scale, y_scale;
        glfwGetWindowContentScale(init_info.window_system->getWindow(), &x_scale, &y_scale);
        float content_scale = fmaxf(1.0f, fmaxf(x_scale, y_scale));
        // windowContentScaleUpdate(content_scale);
        // glfwSetWindowContentScaleCallback(init_info.window_system->getWindow(), windowContentScaleCallback);
        
        // load font for imgui
        ImGuiIO& io = ImGui::GetIO();
        // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        // io.ConfigDockingAlwaysTabBar         = true;
        // io.ConfigWindowsMoveFromTitleBarOnly = true;
        // io.Fonts->AddFontFromFileTTF(
        //     config_manager->getEditorFontPath().generic_string().data(), content_scale * 16, nullptr, nullptr);
        // io.Fonts->Build();

        ImGuiStyle& style     = ImGui::GetStyle();
        style.WindowPadding   = ImVec2(1.0, 0);
        style.FramePadding    = ImVec2(14.0, 2.0f);
        style.ChildBorderSize = 0.0f;
        style.FrameRounding   = 5.0f;
        style.FrameBorderSize = 1.5f;
        // setUIColorStyle();
        
        // initialize imgui vulkan render backend
        init_info.render_system->initializeUIRenderBackend(this);
    }
    void EditorUI::showEditorUI()
    {
        
    }

    void EditorUI::preRender() { showEditorUI(); }
}