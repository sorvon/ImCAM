#pragma once

#include "editor/ui/editor_ui.h"
#include "runtime/cam_engine.h"
#include <glm/glm.hpp>

#include <memory>

namespace ImCAM
{
    class CAMEditor
    {
        friend class EditorUI;
    public:
        CAMEditor();
        virtual ~CAMEditor();
        void initialize(CAMEngine* engine_runtime);
        void clear();

        void run(); 
    protected:
        std::shared_ptr<EditorUI> m_editor_ui;
        CAMEngine* m_engine_runtime{ nullptr };
    };
}