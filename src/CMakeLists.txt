include_directories(${COMPUTELIB_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(core)
add_subdirectory(graph)
add_subdirectory(runtime)

set(VERSION_FILE "arm_compute_version.embed")
add_custom_command(OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/${VERSION_FILE}"
    COMMAND echo '\"arm_compute_version=%s\"' > ${VERSION_FILE}
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "Generating version file"
)

add_custom_target(version_file DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${VERSION_FILE}")
