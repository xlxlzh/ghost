aux_source_directory(./src ENGINE_CODE)
source_group("src" FILES ${ENGINE_CODE})

file(GLOB ENGINE_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
source_group("include" FILES ${ENGINE_HEADERS})

include_directories(include)

add_library(GhostEngine STATIC ${ENGINE_CODE} ${ENGINE_HEADERS})