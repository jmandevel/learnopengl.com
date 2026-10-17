add_executable(3-hello-square "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

target_link_libraries(3-hello-square 
    PRIVATE
        glad
        glfw
        gld
)