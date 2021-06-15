cmake_minimum_required(VERSION 3.13.0)

project(spr
    LANGUAGES C
    DESCRIPTION "Simplified Portable Runtime Library (SPR)"
)

if (BUILD_SHARED_LIBS)
    add_library(${PROJECT_NAME} SHARED)
else()
    add_library(${PROJECT_NAME} STATIC)
endif()

include(cmake/spr_options.cmake)
include(cmake/spr_config.cmake)

target_include_directories(${PROJECT_NAME}
PRIVATE
    include
    ${PROJECT_BINARY_DIR}
)

target_sources(${PROJECT_NAME}
PRIVATE
    lib/spr_array.c
    lib/spr_cpuinfo.c
    lib/spr_dso.c
    lib/spr_errno.c
    lib/spr_filesys.c
    lib/spr_list.c
    lib/spr_string.c
    lib/spr_time.c
    lib/spr_version.c
    lib/memory/spr_memory.c
    lib/memory/spr_pool.c
    lib/network/spr_sockaddr.c
    lib/network/spr_socket.c
    lib/network/spr_sockopt.c
    lib/thread/spr_mutex.c
    lib/thread/spr_semaphore.c
    lib/thread/spr_thread.c
)

target_compile_options(${PROJECT_NAME}
PRIVATE
    -Wall
    -Wextra
    -Wuninitialized
)

target_compile_features(${PROJECT_NAME} PRIVATE c_std_99)

target_compile_definitions(${PROJECT_NAME}
PRIVATE
    SPR_SYSTEM_NAME="${CMAKE_SYSTEM_NAME}"
)

if (NOT CMAKE_BUILD_TYPE)
    set (build_type release)
else()
    string(TOLOWER ${CMAKE_BUILD_TYPE} build_type)
endif()

if (build_type STREQUAL debug)
    target_compile_options(${PROJECT_NAME} PRIVATE -g3 -O0 -ggdb3)
elseif (build_type STREQUAL release)
    target_compile_options(${PROJECT_NAME} PRIVATE -g0 -O3)
endif()

target_link_libraries(${PROJECT_NAME}
    ${CMAKE_REQUIRED_LIBRARIES}
)

if (SPR_POSIX)
    target_link_libraries(${PROJECT_NAME} pthread)
    target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS})
elseif(SPR_WIN32)
    target_link_libraries(${PROJECT_NAME} ws2_32)
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME})
install(FILES build/spr_config.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME})
