set(LIB_GRAPH_DIR ${CMAKE_CURRENT_SOURCE_DIR})

function(add_libgraph_src) # For adding source file inside subdirectories
    file(RELATIVE_PATH name ${LIB_GRAPH_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
    string(REPLACE "/" "_" name ${name})
    set(name graph_${name})
    add_library(${name} OBJECT ${ARGN})
    target_sources(graph PRIVATE $<TARGET_OBJECTS:${name}>)
endfunction()

add_library(graph
    Graph.cpp
    GraphBuilder.cpp
    GraphContext.cpp
    GraphManager.cpp
    INode.cpp
    PassManager.cpp
    Tensor.cpp
    TypeLoader.cpp
    Utils.cpp
    Workload.cpp
)

add_subdirectory(algorithms)
add_subdirectory(backends)
add_subdirectory(detail)
add_subdirectory(frontend)
add_subdirectory(mutators)
add_subdirectory(nodes)
add_subdirectory(printers)
