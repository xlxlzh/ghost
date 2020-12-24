include_directories(../../Engine/include ../../Engine/include/Win32)

if(WIN32)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
endif(WIN32)

link_directories(../../../External/assimp/assimp/lib)

add_executable(SampleApplication main.cpp SampleApplication.cpp SampleApplication.h)

target_link_libraries(SampleApplication GhostEngine)
