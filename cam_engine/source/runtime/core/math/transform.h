#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace ImCAM
{
    class Transform
    {
    public:
        glm::vec3 m_position{0.0f, 0.0f, 0.0f};
        glm::vec3 m_rotation{0.0f, 0.0f, 0.0f};
        glm::vec3 m_scale{1.0f, 1.0f, 1.0f};
        
        Transform() = default;
        Transform(const glm::vec3 postion, const glm::vec3 rotation, const glm::vec3 scale):
            m_position{postion}, m_rotation{rotation}, m_scale{scale} {}
        glm::mat4 getMatrix() const
        {
            glm::mat4 trans{1.0f};
            trans = glm::scale(trans, m_scale);
            glm::qua<float> q = glm::qua<float>(glm::radians(m_rotation));
            trans = glm::mat4_cast(q) * trans;
            trans = glm::translate(trans, m_position);
            return trans;
        }
    };
    
} // namespace ImCAM
