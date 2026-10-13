# hocgv-miso: miso-generated Jacobian validity solvers
# License: see hocgv-miso repo

if(TARGET miso)
    return()
endif()

message(STATUS "Third-party: creating target 'miso' via hocgv-miso")

include(CPM)
# CPMAddPackage("gh:fsichetti/hocgv-miso#aaab372d086608ab717ecb9ba0e7bc0fe00c7a10")
set(POLYFEM_HOCGV_MISO_SOURCE_DIR "" CACHE PATH "Local hocgv-miso degenerate-check checkout")
if(POLYFEM_HOCGV_MISO_SOURCE_DIR)
    add_subdirectory("${POLYFEM_HOCGV_MISO_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/hocgv-miso")
else()
    CPMAddPackage("gh:fsichetti/hocgv-miso#5f60d2aa237f7ea509428ebde247150be21b41e3")
endif()
