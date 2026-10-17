add_executable(5-uniforms "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

target_link_libraries(5-uniforms 
    PRIVATE
        glad
        glfw
        gld
)