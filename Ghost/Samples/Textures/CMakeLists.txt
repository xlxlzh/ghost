include_directories(../../Engine/include ../../Engine/include/Win32 ../../Engine/GUI)

if(WIN32)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
endif(WIN32)

link_directories(../../../External/assimp/assimp/lib)

add_executable(Textures main.cpp TexturesApplication.cpp TexturesApplication.h)

add_custom_command(TARGET Textures POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/GhostConfig.xml ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(Textures GhostEngine)
