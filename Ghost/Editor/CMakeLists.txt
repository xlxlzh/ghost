file(GLOB_RECURSE EDITOR_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
source_group("include" FILES ${EDITOR_HEADERS})

file(GLOB_RECURSE EDITOR_CODES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
source_group("src" FILES ${EDITOR_CODES})

if(WIN32)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
endif(WIN32)

include_directories(../Engine/include ../Engine/GUI)
include_directories(./include)
include_directories(../Engine/include/RHI)
include_directories(../../External/tinyxml2)

link_directories(../../External/assimp/assimp/lib)
link_directories(../../External/SDL2/lib)

add_executable(GhostEditor GhostEditor.cpp ${EDITOR_HEADERS} ${EDITOR_CODES})

target_link_libraries(GhostEditor GhostEngine TinyXML2)

add_custom_command(TARGET GhostEditor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/EditorConfig.xml ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/EditorConfig.xml $<TARGET_FILE_DIR:GhostEditor>)