cmake_minimum_required(VERSION 3.29)
project(dsaproject3)

set(CMAKE_CXX_STANDARD 14)

# If your .h files also live at project root:
include_directories(${CMAKE_SOURCE_DIR})

add_executable(dsaproject3
        main.cpp
        QuickSort.cpp
        MergeSort.cpp
)

# copy the JSON into your build dir
configure_file(
        ${CMAKE_SOURCE_DIR}/super_bowl_ads_updated.json
        ${CMAKE_BINARY_DIR}/super_bowl_ads_updated.json
        COPYONLY
)
