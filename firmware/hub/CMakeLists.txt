cmake_minimum_required(VERSION 3.16)

# Same shared packet definition the node compiles against.
set(EXTRA_COMPONENT_DIRS "../shared")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(beesense_hub)
