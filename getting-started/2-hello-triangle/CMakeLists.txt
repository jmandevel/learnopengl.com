add_executable(2-hello-triangle "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

target_link_libraries(2-hello-triangle 
    PRIVATE
        glad
        glfw
        gld
)