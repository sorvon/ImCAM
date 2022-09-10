#include <cassert>
#include <mutex>

#include "editor_scene_manager.h"

namespace ImCAM
{
    void EditorSceneManager::initialize() {}

    void EditorSceneManager::tick(float delta_time)
    {
        // std::shared_ptr<GObject> selected_gobject = getSelectedGObject().lock();
        // if (selected_gobject)
        // {
        //     TransformComponent* transform_component = selected_gobject->tryGetComponent(TransformComponent);
        //     if (transform_component)
        //     {
        //         transform_component->setDirtyFlag(true);
        //     }
        // }
    }
} // namespace ImCAM
