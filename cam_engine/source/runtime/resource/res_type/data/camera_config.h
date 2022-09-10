#pragma once

#include <glm/glm.hpp>

namespace ImCAM
{
    class CameraPose
    {
    public:
        glm::vec3 m_position;
        glm::vec3 m_target;
        glm::vec3 m_up;
    };

    class CameraConfig
    {
    public:
        CameraPose m_pose;
        glm::vec2    m_aspect;
        float      m_z_far;
        float      m_z_near;
    };
} // namespace ImCAM
