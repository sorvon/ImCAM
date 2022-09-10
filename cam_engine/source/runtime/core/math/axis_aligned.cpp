#include "axis_aligned.h"

namespace ImCAM
{
    AxisAlignedBox::AxisAlignedBox(const glm::vec3& center, const glm::vec3& half_extent) { update(center, half_extent); }

    void AxisAlignedBox::merge(const glm::vec3& new_point)
    {
        m_min_corner = glm::min(m_min_corner, new_point);
        m_max_corner = glm::max(m_max_corner, new_point);

        m_center      = 0.5f * (m_min_corner + m_max_corner);
        m_half_extent = m_center - m_min_corner;
    }

    void AxisAlignedBox::update(const glm::vec3& center, const glm::vec3& half_extent)
    {
        m_center      = center;
        m_half_extent = half_extent;
        m_min_corner  = center - half_extent;
        m_max_corner  = center + half_extent;
    }
} // namespace ImCAM
