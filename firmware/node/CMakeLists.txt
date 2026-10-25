cmake_minimum_required(VERSION 3.16)

# Pulls in firmware/shared/ so the node and the hub compile the same packet
# definition instead of two copies that drift apart.
set(EXTRA_COMPONENT_DIRS "../shared")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(beesense_node)
