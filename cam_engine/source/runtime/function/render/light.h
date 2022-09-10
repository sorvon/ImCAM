#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "runtime/function/render/render_type.h"

#include <vector>

namespace ImCAM
{
    struct PointLight
    {
        glm::vec3 m_position;
        // radiant flux in W
        glm::vec3 m_flux;

        // calculate an appropriate radius for light culling
        // a windowing function in the shader will perform a smooth transition to zero
        // this is not physically based and usually artist controlled
        float calculateRadius() const
        {
            // radius = where attenuation would lead to an intensity of 1W/m^2
            const float INTENSITY_CUTOFF    = 1.0f;
            const float ATTENTUATION_CUTOFF = 0.05f;
            glm::vec3     intensity           = m_flux / (4.0f * glm::pi<float>());
            float       maxIntensity        = glm::max(glm::max(intensity.x, intensity.y), intensity.z);
            float       attenuation = glm::max(INTENSITY_CUTOFF, ATTENTUATION_CUTOFF * maxIntensity) / maxIntensity;
            return 1.0f / sqrtf(attenuation);
        }
    };

    struct AmbientLight
    {
        glm::vec3 m_irradiance;
    };

    struct PDirectionalLight
    {
        glm::vec3 m_direction;
        glm::vec3 m_color;
    };

    struct LightList
    {
        // vertex buffers seem to be aligned to 16 bytes
        struct PointLightVertex
        {
            glm::vec3 m_position;
            float   m_padding;
            // radiant intensity in W/sr
            // can be calculated from radiant flux
            glm::vec3 m_intensity;
            float   m_radius;
        };
    };

    class PointLightList : public LightList
    {
    public:
        void init() {}
        void shutdown() {}

        // upload changes to GPU
        void update() {}

        std::vector<PointLight> m_lights;

        std::shared_ptr<BufferData> m_buffer;
    };

} // namespace ImCAM
