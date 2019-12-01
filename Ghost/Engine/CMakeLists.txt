aux_source_directory(./src ENGINE_CODE)
source_group("src" FILES ${ENGINE_CODE})

aux_source_directory(./src/Win32 WIN32_CODE)
source_group("src\\Win32" FILES ${WIN32_CODE})

file(GLOB ENGINE_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
source_group("include" FILES ${ENGINE_HEADERS})

file(GLOB WIN32_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/Win32/*.h")
source_group("include\\Win32" FILES ${WIN32_HEADERS})

include_directories(include)
include_directories(include/Win32)

add_library(GhostEngine STATIC ${ENGINE_CODE} ${ENGINE_HEADERS} ${WIN32_CODE} ${WIN32_HEADERS})