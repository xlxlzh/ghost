include_directories(../../Engine/include ../../Engine/include/Win32 ../../Engine/GUI)

if(WIN32)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
endif(WIN32)

link_directories(../../../External/assimp/assimp/lib)

add_executable(SampleApplication main.cpp SampleApplication.cpp SampleApplication.h SettingPage.h SettingPage.cpp)

add_custom_command(TARGET SampleApplication POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/GhostConfig.xml ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(SampleApplication GhostEngine)
