add_library(content INTERFACE "")
target_compile_definitions(content INTERFACE CONTENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")
