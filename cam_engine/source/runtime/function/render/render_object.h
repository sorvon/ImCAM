#pragma once

#include <glm/glm.hpp>
#include "runtime/function/framework/object/object_id_allocator.h"

#include <string>
#include <vector>

namespace ImCAM
{
    struct GameObjectMeshDesc
    {
        std::string m_mesh_file;
    };

    struct SkeletonBindingDesc
    {
        std::string m_skeleton_binding_file;
    };

    struct SkeletonAnimationResultTransform
    {
        glm::mat4 m_matrix;
    };

    struct SkeletonAnimationResult
    {
        std::vector<SkeletonAnimationResultTransform> m_transforms;
    };

    struct GameObjectMaterialDesc
    {
        std::string m_base_color_texture_file;
        std::string m_metallic_roughness_texture_file;
        std::string m_normal_texture_file;
        std::string m_occlusion_texture_file;
        std::string m_emissive_texture_file;
        bool        m_with_texture {false};
    };

    struct GameObjectTransformDesc
    {
        glm::mat4 m_transform_matrix {1.0f};
    };

    struct GameObjectPartDesc
    {
        GameObjectMeshDesc      m_mesh_desc;
        GameObjectMaterialDesc  m_material_desc;
        GameObjectTransformDesc m_transform_desc;
        bool                    m_with_animation {false};
        SkeletonBindingDesc     m_skeleton_binding_desc;
        SkeletonAnimationResult m_skeleton_animation_result;
    };

    constexpr size_t k_invalid_part_id = std::numeric_limits<size_t>::max();

    struct GameObjectPartId
    {
        GObjectID m_go_id {k_invalid_gobject_id};
        size_t    m_part_id {k_invalid_part_id};

        bool   operator==(const GameObjectPartId& rhs) const { return m_go_id == rhs.m_go_id && m_part_id == rhs.m_part_id; }
        size_t getHashValue() const { return m_go_id ^ (m_part_id << 1); }
        bool   isValid() const { return m_go_id != k_invalid_gobject_id && m_part_id != k_invalid_part_id; }
    };

    class GameObjectDesc
    {
    public:
        GameObjectDesc() : m_go_id(0) {}
        GameObjectDesc(size_t go_id, const std::vector<GameObjectPartDesc>& parts) :
            m_go_id(go_id), m_object_parts(parts)
        {}

        GObjectID                              getId() const { return m_go_id; }
        const std::vector<GameObjectPartDesc>& getObjectParts() const { return m_object_parts; }

    private:
        GObjectID                       m_go_id {k_invalid_gobject_id};
        std::vector<GameObjectPartDesc> m_object_parts;
    };
} 

template<>
struct std::hash<ImCAM::GameObjectPartId>
{
    size_t operator()(const ImCAM::GameObjectPartId& rhs) const noexcept { return rhs.getHashValue(); }
};
