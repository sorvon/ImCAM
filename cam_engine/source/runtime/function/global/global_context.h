#pragma once

#include "runtime/core/log/log_system.h"
#include "runtime/function/render/render_system.h"
#include "runtime/function/render/window_system.h"

#include <memory>
#include <string>

namespace ImCAM
{
    class LogSystem;
    class RenderSystem;
    class WindowSystem;
    // class InputSystem;
    // class FileSystem;

    // class PhysicsSystem;
    // class PhysicsManager;
    
    // class AssetManager;
    // class ConfigManager;
    // class WorldManager;


    struct EngineInitParams;

    /// Manage the lifetime and creation/destruction order of all global system
    class RuntimeGlobalContext
    {
    public:
        // create all global systems and initialize these systems
        void startSystems(const std::string& config_file_path);
        // destroy all global systems
        void shutdownSystems();

    public:
        std::shared_ptr<LogSystem>      m_logger_system;
        // std::shared_ptr<InputSystem>    m_input_system;
        // std::shared_ptr<FileSystem>     m_file_system;
        // std::shared_ptr<AssetManager>   m_asset_manager;
        // std::shared_ptr<ConfigManager>  m_config_manager;
        // std::shared_ptr<WorldManager>   m_world_manager;
        // std::shared_ptr<PhysicsSystem>  m_legacy_physics_system;
        // std::shared_ptr<PhysicsManager> m_physics_manager;
        std::shared_ptr<WindowSystem>   m_window_system;
        std::shared_ptr<RenderSystem>   m_render_system;
    };

    extern RuntimeGlobalContext g_runtime_global_context;
}