function(add_libgraph_runtime_src) # For adding source file inside subdirectories
    file(RELATIVE_PATH name ${LIB_GRAPH_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
    string(REPLACE "/" "_" name ${name})
    set(name graph_runtime_${name})
    add_library(${name} OBJECT ${ARGN})
    target_sources(graph_runtime PRIVATE $<TARGET_OBJECTS:${name}>)
endfunction()

add_library(graph_runtime
    Allocator.cpp
    BlobLifetimeManager.cpp
    BlobMemoryPool.cpp
    CPUUtils.cpp
    Distribution1D.cpp
    HOG.cpp
    ILutAllocator.cpp
    IScheduler.cpp
    ISimpleLifetimeManager.cpp
    ITensorAllocator.cpp
    Lut.cpp
    LutAllocator.cpp
    MEMUtils.cpp
    Memory.cpp
    MemoryManagerOnDemand.cpp
    MultiHOG.cpp
    MultiImage.cpp
    OffsetLifetimeManager.cpp
    OffsetMemoryPool.cpp
    PoolManager.cpp
    Pyramid.cpp
    Scheduler.cpp
    SubTensor.cpp
    Tensor.cpp
    TensorAllocator.cpp
    Utils.cpp
)

add_dependencies(graph_runtime version_file)

#add_subdirectory(CPP)
