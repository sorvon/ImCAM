#pragma once

#include "runtime/core/math/transform.h"
#include "runtime/function/framework/component/component.h"

namespace ImCAM
{
    class TransformComponent : public Component
    {
    protected:
        Transform m_transform;

        Transform m_transform_buffer[2];
        size_t    m_current_index {0};
        size_t    m_next_index {1};
    public:
        TransformComponent() = default;
        void postLoadResource(std::weak_ptr<GObject> parent_object) override;

        glm::vec3    getPosition() const { return m_transform_buffer[m_current_index].m_position; }
        glm::vec3    getScale() const { return m_transform_buffer[m_current_index].m_scale; }
        glm::vec3 getRotation() const { return m_transform_buffer[m_current_index].m_rotation; }

        void setPosition(const glm::vec3& new_translation);

        void setScale(const glm::vec3& new_scale);

        void setRotation(const glm::vec3& new_rotation);

        const Transform& getTransformConst() const { return m_transform_buffer[m_current_index]; }
        Transform&       getTransform() { return m_transform_buffer[m_next_index]; }

        glm::mat4 getMatrix() const { return m_transform_buffer[m_current_index].getMatrix(); }

        void tick(float delta_time) override;
    };
    
}