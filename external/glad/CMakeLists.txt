# GLAD is added as a library that can be linked with

add_library(glad STATIC)
add_library(glad::glad ALIAS glad)

# Add source files
target_sources(
    glad

    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/glad/include/glad/glad.h

    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/glad/include/glad/KHR/khrplatform.h
    ${CMAKE_CURRENT_LIST_DIR}/glad/src/glad.c)

# Set the include directories
target_include_directories(
    glad
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/glad/include)
