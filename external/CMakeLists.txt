# Add subdirectories with dependencies
add_subdirectory(glad)
add_subdirectory(stb)
include(${CMAKE_CURRENT_LIST_DIR}/glm.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/glfw.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/tinyobjloader.cmake)

# FC is set to be disconnected in order to stop downloading
set(FETCHCONTENT_FULLY_DISCONNECTED OFF CACHE BOOL "set to 'off' to begin downloading again" FORCE)
