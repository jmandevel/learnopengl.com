add_executable(7-single-texture "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

target_link_libraries(7-single-texture 
    PRIVATE
        glad
        glfw
        gld
        stb
        content
)