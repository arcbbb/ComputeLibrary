set(LIB_GRAPH_CORE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

function(add_libgraph_core_src) # For adding source file inside subdirectories
    file(RELATIVE_PATH name ${LIB_GRAPH_CORE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
    string(REPLACE "/" "_" name ${name})
    set(name graph_core_${name})
    add_library(${name} OBJECT ${ARGN})
    target_sources(graph_core PRIVATE $<TARGET_OBJECTS:${name}>)
endfunction()

add_library(graph_core
    AccessWindowAutoPadding.cpp
    AccessWindowStatic.cpp
    AccessWindowTranspose.cpp
    Error.cpp
    GPUTarget.cpp
    HOGInfo.cpp
    Helpers.cpp
    IAccessWindow.cpp
    IDistribution.cpp
    IDistribution1D.cpp
    IKernel.cpp
    ITensor.cpp
    MultiImageInfo.cpp
    PyramidInfo.cpp
    Rounding.cpp
    SubTensorInfo.cpp
    TensorInfo.cpp
    Utils.cpp
    Validate.cpp
)
add_dependencies(graph_core version_file)

#add_subdirectory(CPP)
add_subdirectory(utils)
