# Set up GLFW by fetching it from GitHub

include(FetchContent)
message(STATUS "Getting GLFW from GitHub.")

option(GLFW_BUILD_DOCS "Disable" OFF)
option(GLFW_BUILD_EXAMPLES "Disable" OFF)
option(GLFW_BUILD_TESTS "Disable" OFF)

# Declare where to find glfw and what version to use
FetchContent_Declare(
    glfw_external
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG fb0f2f92a38c1d6a776ffeb253329f8d1c65694c
    GIT_PROGRESS TRUE
)

# Populate and make available
FetchContent_MakeAvailable(glfw_external)

