add_executable(6-shader-class "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

target_link_libraries(6-shader-class 
    PRIVATE
        glad
        glfw
        gld
        getting-started-shaders
)