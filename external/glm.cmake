# Set up GLM by fetching it from GitHub

include(FetchContent)
message(STATUS "Get GLM package from GitHub.")

option(GLM_TEST_ENABLE "Disable" OFF)

# Declare where to find glm and what version to use
FetchContent_Declare(
    glm_external
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    #GIT_TAG 6ad79aae3eb5bf809c30bf1168171e9e55857e45 #newest 
    GIT_TAG 9749727c2db4742369219e1d452f43e918734b4e
    GIT_PROGRESS TRUE)

# Populate it for building
FetchContent_MakeAvailable(glm_external)