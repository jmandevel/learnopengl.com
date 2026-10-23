add_executable(9-colored-double-texture "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

target_link_libraries(9-colored-double-texture 
    PRIVATE
        glad
        glfw
        gld
        stb
        content
)