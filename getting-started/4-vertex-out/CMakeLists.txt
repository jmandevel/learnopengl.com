add_executable(4-vertex-out "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

target_link_libraries(4-vertex-out 
    PRIVATE
        glad
        glfw
        gld
)