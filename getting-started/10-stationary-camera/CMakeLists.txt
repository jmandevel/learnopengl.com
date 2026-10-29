add_executable(10-stationary-camera "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

target_link_libraries(10-stationary-camera
    PRIVATE
        glad
        glfw
        gld
        stb
        glm
        content
)