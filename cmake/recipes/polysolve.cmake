# PolySolve (https://github.com/polyfem/polysolve)
# License: MIT

if(TARGET polysolve)
    return()
endif()

message(STATUS "Third-party: creating target 'polysolve'")

include(CPM)

CPMDeclarePackage(eigen NAME eigen GIT_REPOSITORY https://eigen.googlesource.com/mirror GIT_TAG 5.0.1 DOWNLOAD_ONLY ON)

# Backport the step-acceptance hook and conservative step rounding from 7ab0a759.
find_package(Git REQUIRED)
CPMAddPackage(
    NAME polysolve
    GITHUB_REPOSITORY polyfem/polysolve
    GIT_TAG 621e8fb3889717a33c4fa33a608052e9d7459e63
    PATCH_COMMAND "${GIT_EXECUTABLE}" apply "${CMAKE_CURRENT_LIST_DIR}/../patches/polysolve_step_acceptance.patch"
)
