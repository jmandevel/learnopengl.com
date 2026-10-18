add_executable(8-colored-single-texture "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

target_link_libraries(8-colored-single-texture 
    PRIVATE
        glad
        glfw
        gld
        stb
        content
)