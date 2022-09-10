#include "global_context.h"

namespace ImCAM
{
    RuntimeGlobalContext g_runtime_global_context;
    void RuntimeGlobalContext::startSystems(const std::string& config_file_path)
    {
        m_logger_system = std::make_shared<LogSystem>();

        m_window_system = std::make_shared<WindowSystem>();
        WindowCreateInfo window_create_info;
        m_window_system->initialize(window_create_info);

        m_render_system = std::make_shared<RenderSystem>();
        RenderSystemInitInfo render_init_info;
        render_init_info.window_system = m_window_system;
        m_render_system->initialize(render_init_info);
    }
    void RuntimeGlobalContext::shutdownSystems()
    {
        m_render_system.reset();
        m_logger_system.reset();
        m_window_system.reset();
    }
}