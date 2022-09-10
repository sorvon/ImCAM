#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <mutex>
#include <vector>

namespace ImCAM
{
    enum class RenderCameraType : int
    {
        Editor,
        Motor
    };

    class RenderCamera
    {
    public:
        RenderCameraType m_current_camera_type {RenderCameraType::Editor};

        static const glm::vec3 X, Y, Z;

        glm::vec3 m_position {0.0f, 0.0f, 0.0f};
        glm::quat m_rotation {1, 0, 0, 0};
        glm::quat m_invRotation {1, 0, 0, 0};
        float m_znear {1000.0f};
        float m_zfar {0.1f};
        glm::vec3 m_up_axis {Z};

        static constexpr float MIN_FOV {10.0f};
        static constexpr float MAX_FOV {89.0f};
        static constexpr int   MAIN_VIEW_MATRIX_INDEX {0};

        std::vector<glm::mat4> m_view_matrices {glm::mat4(1.0f)};

        void setCurrentCameraType(RenderCameraType type);
        void setMainViewMatrix(const glm::mat4& view_matrix, RenderCameraType type = RenderCameraType::Editor);

        void move(glm::vec3 delta);
        void rotate(glm::vec2 delta);
        void zoom(float offset);
        void lookAt(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up);

        void setAspect(float aspect);
        void setFOVx(float fovx) { m_fovx = fovx; }

        glm::vec3 position() const { return m_position; }
        glm::quat rotation() const { return m_rotation; }

        glm::vec3 forward() const { return (m_invRotation * Y); }
        glm::vec3 up() const { return (m_invRotation * Z); }
        glm::vec3 right() const { return (m_invRotation * X); }
        glm::vec2 getFOV() const { return {m_fovx, m_fovy}; }
        glm::mat4 getViewMatrix();
        glm::mat4 getPersProjMatrix() const;
        glm::mat4 getLookAtMatrix() const { return glm::lookAt(position(), position() + forward(), up()); }
        float     getFovYDeprecated() const { return m_fovy; }

    protected:
        float m_aspect {0.f};
        float m_fovx {89.f};
        float m_fovy {0.f};

        std::mutex m_view_matrix_mutex;
    };

    inline const glm::vec3 RenderCamera::X = {1.0f, 0.0f, 0.0f};
    inline const glm::vec3 RenderCamera::Y = {0.0f, 1.0f, 0.0f};
    inline const glm::vec3 RenderCamera::Z = {0.0f, 0.0f, 1.0f};

} 