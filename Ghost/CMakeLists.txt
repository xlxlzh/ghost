add_subdirectory(Engine)
add_subdirectory(CopyResources)

if (GHOST_BUILD_SAMPLES)
  add_subdirectory(Samples)
endif(GHOST_BUILD_SAMPLES)

if (GHOST_BUILD_EDITOR)
  add_subdirectory(Editor)
endif(GHOST_BUILD_EDITOR)