#include "render_camera.h"

namespace ImCAM
{
    void RenderCamera::setCurrentCameraType(RenderCameraType type)
    {
        std::lock_guard<std::mutex> lock_guard(m_view_matrix_mutex);
        m_current_camera_type = type;
    }

    void RenderCamera::setMainViewMatrix(const glm::mat4& view_matrix, RenderCameraType type)
    {
        std::lock_guard<std::mutex> lock_guard(m_view_matrix_mutex);
        m_current_camera_type                   = type;
        m_view_matrices[MAIN_VIEW_MATRIX_INDEX] = view_matrix;

        glm::vec3 s  = glm::vec3(view_matrix[0][0], view_matrix[0][1], view_matrix[0][2]);
        glm::vec3 u  = glm::vec3(view_matrix[1][0], view_matrix[1][1], view_matrix[1][2]);
        glm::vec3 f  = glm::vec3(-view_matrix[2][0], -view_matrix[2][1], -view_matrix[2][2]);
        m_position = s * (-view_matrix[0][3]) + u * (-view_matrix[1][3]) + f * view_matrix[2][3];
    }

    void RenderCamera::move(glm::vec3 delta) { m_position += delta; }

    void RenderCamera::rotate(glm::vec2 delta)
    {
        // rotation around x, y axis
        // delta = glm::vec2(Radian(Degree(delta.x)).valueRadians(), Radian(Degree(delta.y)).valueRadians());
        delta = glm::radians(delta);
        // limit pitch
        float dot = glm::dot(m_up_axis, forward());
        if ((dot < -0.99f && delta.x > 0.0f) || // angle nearing 180 degrees
            (dot > 0.99f && delta.x < 0.0f))    // angle nearing 0 degrees
            delta.x = 0.0f;

        // pitch is relative to current sideways rotation
        // yaw happens independently
    // this prevents roll
        glm::quat pitch, yaw;
        pitch = glm::quat(glm::vec3(delta.x, 0.0f, 0.0f));
        yaw = glm::quat(glm::vec3(0.0f, 0.0f, delta.y));
        // pitch.fromAngleAxis(Radian(delta.x), X);
        // yaw.fromAngleAxis(Radian(delta.y), Z);

        m_rotation = pitch * m_rotation * yaw;
        
        m_invRotation = glm::conjugate(m_rotation);
    }

    void RenderCamera::zoom(float offset)
    {
        // > 0 = zoom in (decrease FOV by <offset> angles)
        m_fovx = glm::clamp(m_fovx - offset, MIN_FOV, MAX_FOV);
    }

    void RenderCamera::lookAt(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up)
    {
        m_position = position;

        // model rotation
        // maps vectors to camera space (x, y, z)
        glm::vec3 forward = glm::normalize(target - position);
        m_rotation = glm::quat(forward, Y);
        // m_rotation      = forward.getRotationTo(Y);

        // correct the up vector
        // the cross product of non-orthogonal vectors is not normalized
        
        glm::vec3 right  = glm::normalize(glm::cross(forward, glm::normalize(up)));
        glm::vec3 orthUp = glm::cross(right, forward);

        glm::quat upRotation = glm::quat(m_rotation * orthUp, Z);
        // glm::quat upRotation = (m_rotation * orthUp).getRotationTo(Z);

        m_rotation = upRotation * m_rotation;

        // inverse of the model rotation
        // maps camera space vectors to model vectors
        m_invRotation = glm::conjugate(m_rotation);
    }

    glm::mat4 RenderCamera::getViewMatrix()
    {
        std::lock_guard<std::mutex> lock_guard(m_view_matrix_mutex);
        glm::mat4 view_matrix(1.0f);
        switch (m_current_camera_type)
        {
            case RenderCameraType::Editor:
                view_matrix = glm::lookAt(position(), position() + forward(), up());
                break;
            case RenderCameraType::Motor:
                view_matrix = m_view_matrices[MAIN_VIEW_MATRIX_INDEX];
                break;
            default:
                break;
        }
        return view_matrix;
    }

    glm::mat4 RenderCamera::getPersProjMatrix() const
    {
        glm::mat4 fix_mat(1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
        
        glm::mat4 proj_mat = fix_mat * glm::perspective(glm::radians(m_fovy), m_aspect, m_znear, m_zfar);

        return proj_mat;
    }

    void RenderCamera::setAspect(float aspect)
    {
        m_aspect = aspect;

        // 1 / tan(fovy * 0.5) / aspect = 1 / tan(fovx * 0.5)
        // 1 / tan(fovy * 0.5) = aspect / tan(fovx * 0.5)
        // tan(fovy * 0.5) = tan(fovx * 0.5) / aspect

        m_fovy = glm::atan(glm::tan(glm::radians(m_fovx) * 0.5f) / m_aspect) * 2.0f;
    }
} // namespace Piccolo
