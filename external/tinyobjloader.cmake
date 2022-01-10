# Set up GLFW by fetching it from GitHub

include(FetchContent)
message(STATUS "Getting tinyobjloader from GitHub.")

option(TINYOBJLOADER_BUILD_TEST_LOADER "Build Example Loader Application" OFF)

# Declare where to find glfw and what version to use
FetchContent_Declare(
    tinyobjloader_external
    GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
    GIT_TAG 097ad6a826336a76e42a27e6e531da7dbdf3fd01
    GIT_PROGRESS TRUE
)

# Populate and make available
FetchContent_MakeAvailable(tinyobjloader_external)

