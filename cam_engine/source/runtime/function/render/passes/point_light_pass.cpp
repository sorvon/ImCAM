// #include "point_light_pass.h"
// #include "runtime/function/render/render_helper.h"
// #include "runtime/function/render/render_mesh.h"
// #include "runtime/function/render/rhi/vulkan/vulkan_rhi.h"
// #include "runtime/function/render/rhi/vulkan/vulkan_util.h"
// #include <glm/glm.hpp>
// #include <map>
// #include <stdexcept>
// #include <vector>
// namespace ImCAM
// {
//     void PointLightShadowPass::initialize(const RenderPassInitInfo* init_info)
//     {
//         RenderPass::initialize(nullptr);

//         setupAttachments();
//         setupRenderPass();
//         setupFramebuffer();
//         setupDescriptorSetLayout();
//     }

//     void PointLightShadowPass::postInitialize()
//     {
//         setupPipelines();
//         setupDescriptorSet();
//     }

//     void PointLightShadowPass::preparePassData(std::shared_ptr<RenderResourceBase> render_resource)
//     {
//         const RenderResource* vulkan_resource = static_cast<const RenderResource*>(render_resource.get());
//         if (vulkan_resource)
//         {
//             m_mesh_point_light_shadow_perframe_storage_buffer_object =
//                 vulkan_resource->m_mesh_point_light_shadow_perframe_storage_buffer_object;
//         }
//     }

//     void PointLightShadowPass::draw()
//     {
//         if (m_vulkan_rhi->isDebugLabelEnabled())
//         {
//             VkDebugUtilsLabelEXT label_info = {
//                 VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT, NULL, "Point Light Shadow", {1.0f, 1.0f, 1.0f, 1.0f}};
//             m_vulkan_rhi->m_vk_cmd_begin_debug_utils_label_ext(m_vulkan_rhi->m_current_command_buffer, &label_info);
//         }

//         drawModel();

//         if (m_vulkan_rhi->isDebugLabelEnabled())
//         {
//             m_vulkan_rhi->m_vk_cmd_end_debug_utils_label_ext(m_vulkan_rhi->m_current_command_buffer);
//         }
//     }

//     void PointLightShadowPass::setupAttachments()
//     {
//         // color and depth
//         m_framebuffer.attachments.resize(2);

//         // color
//         m_framebuffer.attachments[0].format = VK_FORMAT_R32_SFLOAT;
//         VulkanUtil::createImage(m_vulkan_rhi->m_physical_device,
//                                 m_vulkan_rhi->m_device,
//                                 m_point_light_shadow_map_dimension,
//                                 m_point_light_shadow_map_dimension,
//                                 m_framebuffer.attachments[0].format,
//                                 VK_IMAGE_TILING_OPTIMAL,
//                                 VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
//                                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
//                                 m_framebuffer.attachments[0].image,
//                                 m_framebuffer.attachments[0].mem,
//                                 0,
//                                 2 * m_max_point_light_count,
//                                 1);
//         m_framebuffer.attachments[0].view = VulkanUtil::createImageView(m_vulkan_rhi->m_device,
//                                                                         m_framebuffer.attachments[0].image,
//                                                                         m_framebuffer.attachments[0].format,
//                                                                         VK_IMAGE_ASPECT_COLOR_BIT,
//                                                                         VK_IMAGE_VIEW_TYPE_2D_ARRAY,
//                                                                         2 * m_max_point_light_count,
//                                                                         1);

//         // depth
//         m_framebuffer.attachments[1].format = m_vulkan_rhi->m_depth_image_format;
//         VulkanUtil::createImage(m_vulkan_rhi->m_physical_device,
//                                 m_vulkan_rhi->m_device,
//                                 m_point_light_shadow_map_dimension,
//                                 m_point_light_shadow_map_dimension,
//                                 m_framebuffer.attachments[1].format,
//                                 VK_IMAGE_TILING_OPTIMAL,
//                                 VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT,
//                                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
//                                 m_framebuffer.attachments[1].image,
//                                 m_framebuffer.attachments[1].mem,
//                                 0,
//                                 2 * m_max_point_light_count,
//                                 1);
//         m_framebuffer.attachments[1].view = VulkanUtil::createImageView(m_vulkan_rhi->m_device,
//                                                                         m_framebuffer.attachments[1].image,
//                                                                         m_framebuffer.attachments[1].format,
//                                                                         VK_IMAGE_ASPECT_DEPTH_BIT,
//                                                                         VK_IMAGE_VIEW_TYPE_2D_ARRAY,
//                                                                         2 * m_max_point_light_count,
//                                                                         1);
//     }

//     void PointLightShadowPass::setupRenderPass()
//     {
//         VkAttachmentDescription attachments[2] = {};

//         VkAttachmentDescription& point_light_shadow_color_attachment_description = attachments[0];
//         point_light_shadow_color_attachment_description.format                   = m_framebuffer.attachments[0].format;
//         point_light_shadow_color_attachment_description.samples                  = VK_SAMPLE_COUNT_1_BIT;
//         point_light_shadow_color_attachment_description.loadOp                   = VK_ATTACHMENT_LOAD_OP_CLEAR;
//         point_light_shadow_color_attachment_description.storeOp                  = VK_ATTACHMENT_STORE_OP_STORE;
//         point_light_shadow_color_attachment_description.stencilLoadOp            = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//         point_light_shadow_color_attachment_description.stencilStoreOp           = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//         point_light_shadow_color_attachment_description.initialLayout            = VK_IMAGE_LAYOUT_UNDEFINED;
//         point_light_shadow_color_attachment_description.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

//         VkAttachmentDescription& point_light_shadow_depth_attachment_description = attachments[1];
//         point_light_shadow_depth_attachment_description.format                   = m_framebuffer.attachments[1].format;
//         point_light_shadow_depth_attachment_description.samples                  = VK_SAMPLE_COUNT_1_BIT;
//         point_light_shadow_depth_attachment_description.loadOp                   = VK_ATTACHMENT_LOAD_OP_CLEAR;
//         point_light_shadow_depth_attachment_description.storeOp                  = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//         point_light_shadow_depth_attachment_description.stencilLoadOp            = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//         point_light_shadow_depth_attachment_description.stencilStoreOp           = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//         point_light_shadow_depth_attachment_description.initialLayout            = VK_IMAGE_LAYOUT_UNDEFINED;
//         point_light_shadow_depth_attachment_description.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

//         VkSubpassDescription subpasses[1] = {};

//         VkAttachmentReference shadow_pass_color_attachment_reference {};
//         shadow_pass_color_attachment_reference.attachment =
//             &point_light_shadow_color_attachment_description - attachments;
//         shadow_pass_color_attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

//         VkAttachmentReference shadow_pass_depth_attachment_reference {};
//         shadow_pass_depth_attachment_reference.attachment =
//             &point_light_shadow_depth_attachment_description - attachments;
//         shadow_pass_depth_attachment_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

//         VkSubpassDescription& shadow_pass   = subpasses[0];
//         shadow_pass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
//         shadow_pass.colorAttachmentCount    = 1;
//         shadow_pass.pColorAttachments       = &shadow_pass_color_attachment_reference;
//         shadow_pass.pDepthStencilAttachment = &shadow_pass_depth_attachment_reference;

//         VkSubpassDependency dependencies[1] = {};

//         VkSubpassDependency& lighting_pass_dependency = dependencies[0];
//         lighting_pass_dependency.srcSubpass           = 0;
//         lighting_pass_dependency.dstSubpass           = VK_SUBPASS_EXTERNAL;
//         lighting_pass_dependency.srcStageMask         = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//         lighting_pass_dependency.dstStageMask         = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
//         lighting_pass_dependency.srcAccessMask        = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT; // STORE_OP_STORE
//         lighting_pass_dependency.dstAccessMask        = 0;
//         lighting_pass_dependency.dependencyFlags      = 0; // NOT BY REGION

//         VkRenderPassCreateInfo renderpass_create_info {};
//         renderpass_create_info.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
//         renderpass_create_info.attachmentCount = (sizeof(attachments) / sizeof(attachments[0]));
//         renderpass_create_info.pAttachments    = attachments;
//         renderpass_create_info.subpassCount    = (sizeof(subpasses) / sizeof(subpasses[0]));
//         renderpass_create_info.pSubpasses      = subpasses;
//         renderpass_create_info.dependencyCount = (sizeof(dependencies) / sizeof(dependencies[0]));
//         renderpass_create_info.pDependencies   = dependencies;

//         if (vkCreateRenderPass(m_vulkan_rhi->m_device, &renderpass_create_info, nullptr, &m_framebuffer.render_pass) !=
//             VK_SUCCESS)
//         {
//             throw std::runtime_error("create point light shadow render pass");
//         }
//     }

//     void PointLightShadowPass::setupFramebuffer()
//     {
//         VkImageView attachments[2] = {m_framebuffer.attachments[0].view, m_framebuffer.attachments[1].view};

//         VkFramebufferCreateInfo framebuffer_create_info {};
//         framebuffer_create_info.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
//         framebuffer_create_info.flags           = 0U;
//         framebuffer_create_info.renderPass      = m_framebuffer.render_pass;
//         framebuffer_create_info.attachmentCount = (sizeof(attachments) / sizeof(attachments[0]));
//         framebuffer_create_info.pAttachments    = attachments;
//         framebuffer_create_info.width           = m_point_light_shadow_map_dimension;
//         framebuffer_create_info.height          = m_point_light_shadow_map_dimension;
//         framebuffer_create_info.layers          = 2 * m_max_point_light_count;

//         if (vkCreateFramebuffer(m_vulkan_rhi->m_device, &framebuffer_create_info, nullptr, &m_framebuffer.framebuffer) !=
//             VK_SUCCESS)
//         {
//             throw std::runtime_error("create point light shadow framebuffer");
//         }
//     }

//     void PointLightShadowPass::setupDescriptorSetLayout()
//     {
//         m_descriptor_infos.resize(1);

//         VkDescriptorSetLayoutBinding mesh_point_light_shadow_global_layout_bindings[3];

//         VkDescriptorSetLayoutBinding& mesh_point_light_shadow_global_layout_perframe_storage_buffer_binding =
//             mesh_point_light_shadow_global_layout_bindings[0];
//         mesh_point_light_shadow_global_layout_perframe_storage_buffer_binding.binding = 0;
//         mesh_point_light_shadow_global_layout_perframe_storage_buffer_binding.descriptorType =
//             VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
//         mesh_point_light_shadow_global_layout_perframe_storage_buffer_binding.descriptorCount = 1;
//         mesh_point_light_shadow_global_layout_perframe_storage_buffer_binding.stageFlags =
//             VK_SHADER_STAGE_GEOMETRY_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

//         VkDescriptorSetLayoutBinding& mesh_point_light_shadow_global_layout_perdrawcall_storage_buffer_binding =
//             mesh_point_light_shadow_global_layout_bindings[1];
//         mesh_point_light_shadow_global_layout_perdrawcall_storage_buffer_binding.binding = 1;
//         mesh_point_light_shadow_global_layout_perdrawcall_storage_buffer_binding.descriptorType =
//             VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
//         mesh_point_light_shadow_global_layout_perdrawcall_storage_buffer_binding.descriptorCount = 1;
//         mesh_point_light_shadow_global_layout_perdrawcall_storage_buffer_binding.stageFlags =
//             VK_SHADER_STAGE_VERTEX_BIT;

//         VkDescriptorSetLayoutBinding&
//             mesh_point_light_shadow_global_layout_per_drawcall_vertex_blending_storage_buffer_binding =
//                 mesh_point_light_shadow_global_layout_bindings[2];
//         mesh_point_light_shadow_global_layout_per_drawcall_vertex_blending_storage_buffer_binding.binding = 2;
//         mesh_point_light_shadow_global_layout_per_drawcall_vertex_blending_storage_buffer_binding.descriptorType =
//             VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
//         mesh_point_light_shadow_global_layout_per_drawcall_vertex_blending_storage_buffer_binding.descriptorCount = 1;
//         mesh_point_light_shadow_global_layout_per_drawcall_vertex_blending_storage_buffer_binding.stageFlags =
//             VK_SHADER_STAGE_VERTEX_BIT;

//         VkDescriptorSetLayoutCreateInfo mesh_point_light_shadow_global_layout_create_info;
//         mesh_point_light_shadow_global_layout_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
//         mesh_point_light_shadow_global_layout_create_info.pNext = NULL;
//         mesh_point_light_shadow_global_layout_create_info.flags = 0;
//         mesh_point_light_shadow_global_layout_create_info.bindingCount =
//             (sizeof(mesh_point_light_shadow_global_layout_bindings) /
//              sizeof(mesh_point_light_shadow_global_layout_bindings[0]));
//         mesh_point_light_shadow_global_layout_create_info.pBindings = mesh_point_light_shadow_global_layout_bindings;

//         if (VK_SUCCESS != vkCreateDescriptorSetLayout(m_vulkan_rhi->m_device,
//                                                       &mesh_point_light_shadow_global_layout_create_info,
//                                                       NULL,
//                                                       &m_descriptor_infos[0].layout))
//         {
//             throw std::runtime_error("create mesh point light shadow global layout");
//         }
//     }

//     void PointLightShadowPass::setupPipelines()
//     {
//         if (!m_vulkan_rhi->isPointLightShadowEnabled())
//             return;

//         m_render_pipelines.resize(1);

//         VkDescriptorSetLayout      descriptorset_layouts[] = {m_descriptor_infos[0].layout, m_per_mesh_layout};
//         VkPipelineLayoutCreateInfo pipeline_layout_create_info {};
//         pipeline_layout_create_info.sType          = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
//         pipeline_layout_create_info.setLayoutCount = (sizeof(descriptorset_layouts) / sizeof(descriptorset_layouts[0]));
//         pipeline_layout_create_info.pSetLayouts    = descriptorset_layouts;

//         if (vkCreatePipelineLayout(
//                 m_vulkan_rhi->m_device, &pipeline_layout_create_info, nullptr, &m_render_pipelines[0].layout) !=
//             VK_SUCCESS)
//         {
//             throw std::runtime_error("create mesh point light shadow pipeline layout");
//         }

//         VkShaderModule vert_shader_module =
//             VulkanUtil::createShaderModule(m_vulkan_rhi->m_device, MESH_POINT_LIGHT_SHADOW_VERT);
//         VkShaderModule geom_shader_module =
//             VulkanUtil::createShaderModule(m_vulkan_rhi->m_device, MESH_POINT_LIGHT_SHADOW_GEOM);
//         VkShaderModule frag_shader_module =
//             VulkanUtil::createShaderModule(m_vulkan_rhi->m_device, MESH_POINT_LIGHT_SHADOW_FRAG);

//         VkPipelineShaderStageCreateInfo vert_pipeline_shader_stage_create_info {};
//         vert_pipeline_shader_stage_create_info.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//         vert_pipeline_shader_stage_create_info.stage  = VK_SHADER_STAGE_VERTEX_BIT;
//         vert_pipeline_shader_stage_create_info.module = vert_shader_module;
//         vert_pipeline_shader_stage_create_info.pName  = "main";

//         VkPipelineShaderStageCreateInfo geom_pipeline_shader_stage_create_info {};
//         geom_pipeline_shader_stage_create_info.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//         geom_pipeline_shader_stage_create_info.stage  = VK_SHADER_STAGE_GEOMETRY_BIT;
//         geom_pipeline_shader_stage_create_info.module = geom_shader_module;
//         geom_pipeline_shader_stage_create_info.pName  = "main";

//         VkPipelineShaderStageCreateInfo frag_pipeline_shader_stage_create_info {};
//         frag_pipeline_shader_stage_create_info.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//         frag_pipeline_shader_stage_create_info.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
//         frag_pipeline_shader_stage_create_info.module = frag_shader_module;
//         frag_pipeline_shader_stage_create_info.pName  = "main";

//         VkPipelineShaderStageCreateInfo shader_stages[] = {vert_pipeline_shader_stage_create_info,
//                                                            geom_pipeline_shader_stage_create_info,
//                                                            frag_pipeline_shader_stage_create_info};

//         auto                                 vertex_binding_descriptions   = MeshVertex::getBindingDescriptions();
//         auto                                 vertex_attribute_descriptions = MeshVertex::getAttributeDescriptions();
//         VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info {};
//         vertex_input_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
//         vertex_input_state_create_info.vertexBindingDescriptionCount   = 1;
//         vertex_input_state_create_info.pVertexBindingDescriptions      = &vertex_binding_descriptions[0];
//         vertex_input_state_create_info.vertexAttributeDescriptionCount = 1;
//         vertex_input_state_create_info.pVertexAttributeDescriptions    = &vertex_attribute_descriptions[0];

//         VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info {};
//         input_assembly_create_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
//         input_assembly_create_info.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
//         input_assembly_create_info.primitiveRestartEnable = VK_FALSE;

//         VkViewport viewport = {0, 0, m_point_light_shadow_map_dimension, m_point_light_shadow_map_dimension, 0.0, 1.0};
//         VkRect2D   scissor  = {{0, 0}, {m_point_light_shadow_map_dimension, m_point_light_shadow_map_dimension}};

//         VkPipelineViewportStateCreateInfo viewport_state_create_info {};
//         viewport_state_create_info.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
//         viewport_state_create_info.viewportCount = 1;
//         viewport_state_create_info.pViewports    = &m_vulkan_rhi->m_viewport;
//         viewport_state_create_info.scissorCount  = 1;
//         viewport_state_create_info.pScissors     = &m_vulkan_rhi->m_scissor;

//         VkPipelineRasterizationStateCreateInfo rasterization_state_create_info {};
//         rasterization_state_create_info.sType            = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
//         rasterization_state_create_info.depthClampEnable = VK_FALSE;
//         rasterization_state_create_info.rasterizerDiscardEnable = VK_FALSE;
//         rasterization_state_create_info.polygonMode             = VK_POLYGON_MODE_FILL;
//         rasterization_state_create_info.lineWidth               = 1.0f;
//         // TODO : test more to verify
//         rasterization_state_create_info.cullMode                = VK_CULL_MODE_BACK_BIT;
//         rasterization_state_create_info.frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE;
//         rasterization_state_create_info.depthBiasEnable         = VK_FALSE;
//         rasterization_state_create_info.depthBiasConstantFactor = 0.0f;
//         rasterization_state_create_info.depthBiasClamp          = 0.0f;
//         rasterization_state_create_info.depthBiasSlopeFactor    = 0.0f;

//         VkPipelineMultisampleStateCreateInfo multisample_state_create_info {};
//         multisample_state_create_info.sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
//         multisample_state_create_info.sampleShadingEnable  = VK_FALSE;
//         multisample_state_create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

//         VkPipelineColorBlendAttachmentState color_blend_attachment_state {};
//         color_blend_attachment_state.colorWriteMask =
//             VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
//         color_blend_attachment_state.blendEnable         = VK_FALSE;
//         color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
//         color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
//         color_blend_attachment_state.colorBlendOp        = VK_BLEND_OP_ADD;
//         color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
//         color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
//         color_blend_attachment_state.alphaBlendOp        = VK_BLEND_OP_ADD;

//         VkPipelineColorBlendStateCreateInfo color_blend_state_create_info {};
//         color_blend_state_create_info.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
//         color_blend_state_create_info.logicOpEnable     = VK_FALSE;
//         color_blend_state_create_info.logicOp           = VK_LOGIC_OP_COPY;
//         color_blend_state_create_info.attachmentCount   = 1;
//         color_blend_state_create_info.pAttachments      = &color_blend_attachment_state;
//         color_blend_state_create_info.blendConstants[0] = 0.0f;
//         color_blend_state_create_info.blendConstants[1] = 0.0f;
//         color_blend_state_create_info.blendConstants[2] = 0.0f;
//         color_blend_state_create_info.blendConstants[3] = 0.0f;

//         VkPipelineDepthStencilStateCreateInfo depth_stencil_create_info {};
//         depth_stencil_create_info.sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
//         depth_stencil_create_info.depthTestEnable       = VK_TRUE;
//         depth_stencil_create_info.depthWriteEnable      = VK_TRUE;
//         depth_stencil_create_info.depthCompareOp        = VK_COMPARE_OP_LESS;
//         depth_stencil_create_info.depthBoundsTestEnable = VK_FALSE;
//         depth_stencil_create_info.stencilTestEnable     = VK_FALSE;

//         VkPipelineDynamicStateCreateInfo dynamic_state_create_info {};
//         dynamic_state_create_info.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
//         dynamic_state_create_info.dynamicStateCount = 0;
//         dynamic_state_create_info.pDynamicStates    = NULL;

//         VkGraphicsPipelineCreateInfo pipelineInfo {};
//         pipelineInfo.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
//         pipelineInfo.stageCount          = (sizeof(shader_stages) / sizeof(shader_stages[0]));
//         pipelineInfo.pStages             = shader_stages;
//         pipelineInfo.pVertexInputState   = &vertex_input_state_create_info;
//         pipelineInfo.pInputAssemblyState = &input_assembly_create_info;
//         pipelineInfo.pViewportState      = &viewport_state_create_info;
//         pipelineInfo.pRasterizationState = &rasterization_state_create_info;
//         pipelineInfo.pMultisampleState   = &multisample_state_create_info;
//         pipelineInfo.pColorBlendState    = &color_blend_state_create_info;
//         pipelineInfo.pDepthStencilState  = &depth_stencil_create_info;
//         pipelineInfo.layout              = m_render_pipelines[0].layout;
//         pipelineInfo.renderPass          = m_framebuffer.render_pass;
//         pipelineInfo.subpass             = 0;
//         pipelineInfo.basePipelineHandle  = VK_NULL_HANDLE;
//         pipelineInfo.pDynamicState       = &dynamic_state_create_info;

//         if (vkCreateGraphicsPipelines(
//                 m_vulkan_rhi->m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_render_pipelines[0].pipeline) !=
//             VK_SUCCESS)
//         {
//             throw std::runtime_error("create mesh point light shadow graphics pipeline");
//         }

//         vkDestroyShaderModule(m_vulkan_rhi->m_device, vert_shader_module, nullptr);
//         vkDestroyShaderModule(m_vulkan_rhi->m_device, geom_shader_module, nullptr);
//         vkDestroyShaderModule(m_vulkan_rhi->m_device, frag_shader_module, nullptr);
//     }





// } // namespace ImCAM
