#include "object.h"
#include "runtime/cam_engine.h"
#include "runtime/function/global/global_context.h"
namespace ImCAM
{
    bool shouldComponentTick(std::string component_type_name)
    {
        if (g_is_editor_mode)
        {
            return g_editor_tick_component_types.find(component_type_name) != g_editor_tick_component_types.end();
        }
        else
        {
            return true;
        }
    }

    void GObject::tick(float delta_time)
    {
        for (auto& component : m_components)
        {
            // if (shouldComponentTick(component.getTypeName()))
            {
                component->tick(delta_time);
            }
        }
    }
} // namespace ImCAM
