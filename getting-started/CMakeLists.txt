add_library(getting-started-shaders INTERFACE "")

target_compile_definitions(getting-started-shaders INTERFACE SHADER_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/shaders/")

add_subdirectory(1-hello-window)
add_subdirectory(2-hello-triangle)
add_subdirectory(3-hello-square)
add_subdirectory(4-vertex-out)
add_subdirectory(5-uniforms)
add_subdirectory(6-shader-class)