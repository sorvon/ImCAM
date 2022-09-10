include(${PROJECT_SOURCE_DIR}/cmake/ShaderCompile.cmake)

file(GLOB_RECURSE GLSL_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/source/*.*"
)  
find_program(GLSL_VALIDATOR glslangValidator)
message(${GLSL_VALIDATOR})
compile_shader(
    "${GLSL_SOURCE_FILES}"
    SHADER_TARGET
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/output
    ${GLSL_VALIDATOR}
)