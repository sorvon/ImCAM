#pragma once

#include "object_id_allocator.h"
#include "runtime/function/framework/component/component.h"
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>
#include <glm/glm.hpp>

namespace ImCAM
{
    class GObject
    {
        typedef std::unordered_set<std::string> TypeNameSet;
    public:
        GObject(GObjectID id) : m_id {id} {}
        virtual ~GObject();

        virtual void tick(float delta_time);

        // bool load(const ObjectInstanceRes& object_instance_res);
        // void save(ObjectInstanceRes& out_object_instance_res);

        GObjectID getID() const { return m_id; }

        void               setName(std::string name) { m_name = name; }
        const std::string& getName() const { return m_name; }

        bool hasComponent(const std::string& compenent_type_name) const;

        template<typename TComponent>
        TComponent* tryGetComponent(const std::string& compenent_type_name)
        {
            for (auto& component : m_components)
            {
                if (component.getTypeName() == compenent_type_name)
                {
                    return static_cast<TComponent*>(component.operator->());
                }
            }

            return nullptr;
        }

        template<typename TComponent>
        const TComponent* tryGetComponentConst(const std::string& compenent_type_name) const
        {
            for (const auto& component : m_components)
            {
                if (component.getTypeName() == compenent_type_name)
                {
                    return static_cast<const TComponent*>(component.operator->());
                }
            }
            return nullptr;
        }

    protected:
        GObjectID   m_id {k_invalid_gobject_id};
        std::string m_name;
        std::string m_definition_url;

        // we have to use the ReflectionPtr due to that the components need to be reflected 
        // in editor, and it's polymorphism
        // std::vector<Reflection::ReflectionPtr<Component>> m_components;    
        std::vector<std::shared_ptr<Component>> m_components;
    };
} // namespace ImCAM
