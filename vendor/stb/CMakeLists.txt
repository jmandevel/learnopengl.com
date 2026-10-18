add_library(stb STATIC "")

add_subdirectory(src)

target_include_directories(stb
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)