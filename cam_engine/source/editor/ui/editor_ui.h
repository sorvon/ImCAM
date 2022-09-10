#pragma once

#include "runtime/function/ui/window_ui.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <chrono>
#include <map>
#include <vector>

namespace ImCAM
{
    class PiccoloEditor;
    class WindowSystem;
    class RenderSystem;
    
    class EditorUI : public WindowUI
    {
    public:
        EditorUI();

    private:
        void showEditorUI();
    public:
        virtual void initialize(WindowUIInitInfo init_info) override final;
        virtual void preRender() override final;

    };
}