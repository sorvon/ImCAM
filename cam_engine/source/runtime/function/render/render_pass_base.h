#pragma once

#include "runtime/function/render/rhi/vulkan/vulkan_rhi.h"
#include "runtime/function/ui/window_ui.h"

namespace ImCAM
{
    class RenderResourceBase;

    struct RenderPassInitInfo
    {};

    struct RenderPassCommonInfo
    {
        std::shared_ptr<VulkanRHI>                rhi;
        std::shared_ptr<RenderResourceBase> render_resource;
    };

    class RenderPassBase
    {
    public:
        virtual void initialize(const RenderPassInitInfo* init_info) = 0;
        virtual void postInitialize();
        virtual void setCommonInfo(RenderPassCommonInfo common_info);
        virtual void preparePassData(std::shared_ptr<RenderResourceBase> render_resource);
        virtual void initializeUIRenderBackend(WindowUI* window_ui);

    protected:
        std::shared_ptr<VulkanRHI>                m_rhi;
        std::shared_ptr<RenderResourceBase> m_render_resource;
    };


} // namespace ImCAM
