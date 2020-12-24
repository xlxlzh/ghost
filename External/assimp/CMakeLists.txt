add_custom_target(Assimp ALL)

add_custom_command(TARGET Assimp POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assimp/bin/ ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})