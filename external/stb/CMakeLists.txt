# Library creation for STB image

add_library(stbimage STATIC)
add_library(stbimage::stbimage ALIAS stbimage)

# Add source files
target_sources(
    stbimage

    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include/stb_image.h

    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/stb_image_impl.cpp
    )

# Set include directories
target_include_directories(
    stbimage
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include)
