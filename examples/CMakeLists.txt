include_directories(${COMPUTELIB_INCLUDE_DIRS})

add_executable(graph_mobilenet
    graph_mobilenet.cpp
)
target_link_libraries(graph_mobilenet utils graph_runtime graph graph_core)
