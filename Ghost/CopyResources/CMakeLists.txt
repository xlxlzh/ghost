add_custom_target(CopyResources ALL)

add_custom_command(TARGET CopyResources POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/Ghost/CopyResources/Resources/ ${PROJECT_BINARY_DIR}/Resources/)