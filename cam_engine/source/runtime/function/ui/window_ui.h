#pragma once

#include "runtime/function/render/window_system.h"
#include "runtime/function/render/render_system.h"
#include <memory>

namespace ImCAM
{
    class WindowSystem;
    class RenderSystem;

    struct WindowUIInitInfo
    {
        std::shared_ptr<WindowSystem> window_system;
        std::shared_ptr<RenderSystem> render_system;
    };

    class WindowUI
    {
    public:
        virtual void initialize(WindowUIInitInfo init_info) = 0;
        virtual void preRender() = 0;
    };
}