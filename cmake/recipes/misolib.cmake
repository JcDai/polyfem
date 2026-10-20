# misolib (https://gitlab.com/minimize-solve/misolib)
# License: none specified upstream

if(TARGET misolib::misolib)
    return()
endif()

message(STATUS "Third-party: creating target 'misolib::misolib'")

include(CPM)
# Degenerate inclusions and sampling at points other than the box vertices
# (numSamples), needed by the degenerate checks.
# TODO: pin the commit once branch polyfem-degenerate is pushed.
CPMAddPackage("gl:minimize-solve/misolib#92a034e974e8e566f43cba2585e56f9b726bd6f1")
