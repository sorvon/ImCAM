set(TARGET_NAME ImCAMRuntime)

file(GLOB_RECURSE HEADER_FILES "*.h")
file(GLOB_RECURSE SOURCE_FILES "*.cpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADER_FILES} ${SOURCE_FILES})
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_library(${TARGET_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${TARGET_NAME} PUBLIC
    ${Vulkan_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/cam_engine/source
)

target_link_libraries(${TARGET_NAME} PUBLIC 
    glfw 
    spdlog::spdlog 
    glm::glm
    imgui::imgui
)

add_dependencies(${TARGET_NAME} SHADER_TARGET)
# message(${PROJECT_SOURCE_DIR}/cam_engine/source)
