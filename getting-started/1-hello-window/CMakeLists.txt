add_executable(1-hello-window "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

target_link_libraries(1-hello-window 
    PRIVATE
        glad
        glfw
        gld
)