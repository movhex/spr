option(OPTION_POOL_THREAD_SAFETY "Pool is thread safety" ON)
option(OPTION_POOL_USES_MMAP "Pool uses mmap" OFF)
option(BUILD_SHARED_LIBS "Build shared instead of static libraries" ON)

if (OPTION_POOL_THREAD_SAFETY)
    target_compile_definitions(${PROJECT_NAME} PRIVATE SPR_POOL_THREAD_SAFETY)
endif()

if (OPTION_POOL_USES_MMAP)
    target_compile_definitions(${PROJECT_NAME} PRIVATE SPR_POOL_USES_MMAP)
endif()

