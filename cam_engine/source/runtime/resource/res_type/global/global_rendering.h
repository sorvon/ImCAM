#pragma once

#include "runtime/resource/res_type/data/camera_config.h"
#include <glm/glm.hpp>
#include <string>
namespace ImCAM
{

    class SkyBoxIrradianceMap
    {
    public:
        std::string m_negative_x_map;
        std::string m_positive_x_map;
        std::string m_negative_y_map;
        std::string m_positive_y_map;
        std::string m_negative_z_map;
        std::string m_positive_z_map;
    };


    class SkyBoxSpecularMap
    {
    public:
        std::string m_negative_x_map;
        std::string m_positive_x_map;
        std::string m_negative_y_map;
        std::string m_positive_y_map;
        std::string m_negative_z_map;
        std::string m_positive_z_map;
    };

    class DirectionalLight
    {
    public:
        glm::vec3 m_direction;
        glm::vec3   m_color;
    };

    class GlobalRenderingRes
    {
    public:
        bool                m_enable_fxaa {false};
        SkyBoxIrradianceMap m_skybox_irradiance_map;
        SkyBoxSpecularMap   m_skybox_specular_map;
        std::string         m_brdf_map;
        std::string         m_color_grading_map;

        glm::vec3            m_sky_color;
        glm::vec3            m_ambient_light;
        CameraConfig     m_camera_config;
        DirectionalLight m_directional_light;
    };
} // namespace ImCAM
