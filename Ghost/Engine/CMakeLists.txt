aux_source_directory(./src ENGINE_CODE)
source_group("src" FILES ${ENGINE_CODE})

aux_source_directory(./src/Win32 WIN32_CODE)
source_group("src\\Win32" FILES ${WIN32_CODE})

aux_source_directory(./src/D3D11 D3D11_CODE)
source_group("src\\D3D11" FILES ${D3D11_CODE})

aux_source_directory(./src/D3D12 D3D12_CODE)
source_group("src\\D3D12" FILES ${D3D12_CODE})

file(GLOB ENGINE_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
source_group("include" FILES ${ENGINE_HEADERS})

file(GLOB WIN32_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/Win32/*.h")
source_group("include\\Win32" FILES ${WIN32_HEADERS})

file(GLOB D3D11_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/D3D11/*.h")
source_group("include\\D3D11" FILES ${D3D11_HEADERS})

file(GLOB D3D12_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/D3D12/*.h")
source_group("include\\D3D12" FILES ${D3D12_HEADERS})

add_definitions(-DGHOST_EXPORT)

find_package(DirectX11 REQUIRED)
include_directories(${DirectX11_INCLUDE_DIR})

include_directories(include)
include_directories(include/Win32)
include_directories(include/D3D11)
include_directories(include/D3D12)

include_directories(../../External/stb)

add_library(GhostEngine SHARED ${ENGINE_CODE} ${ENGINE_HEADERS} ${WIN32_CODE} ${WIN32_HEADERS} ${D3D11_HEADERS} ${D3D11_CODE} ${D3D12_HEADERS} ${D3D12_CODE})

target_link_libraries(GhostEngine ${DirectX11_LIBRARY})