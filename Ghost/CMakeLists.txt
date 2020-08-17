add_subdirectory(Engine)
add_subdirectory(CopyResources)

if (GHOST_SAMPLES_BUILD)
  add_subdirectory(Samples)
endif(GHOST_SAMPLES_BUILD)