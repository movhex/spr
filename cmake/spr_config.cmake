include(CheckCSourceCompiles)

check_c_source_compiles("
int main(void) {
#if defined(__linux__) || defined(__gnu_linux__)
#elif
    generete_error;
#endif
}" SPR_LINUX)

check_c_source_compiles("
int main(void) {
#if defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
#elif
    generete_error();
#endif
}" SPR_FREEBSD)

check_c_source_compiles("
int main(void) {
#if defined(__APPLE__) || defined(__MACH__)
#elif
    generete_error();
#endif
}" SPR_DARWIN)

check_c_source_compiles("
int main(void) {
#if defined(__sun) && defined(__SVR4)
#elif
    generete_error();
#endif
}" SPR_SOLARIS)

check_c_source_compiles("
int main(void) {
#if defined(_WIN32) || defined(__WIN32__)
#elif
    generete_error();
#endif
}" SPR_WIN32)

if (NOT(SPR_LINUX OR
        SPR_FREEBSD OR
        SPR_DARWIN OR
        SPR_SOLARIS OR
        SPR_WIN32))
    message(FATAL_ERROR "Not supported platform")
endif()

if (SPR_LINUX OR SPR_FREEBSD OR SPR_DARWIN OR SPR_SOLARIS)
    set(SPR_POSIX 1)
endif()


check_c_source_compiles("
#include <stddef.h>
#include <sys/mman.h>
int main(void) {
    mmap(NULL, 100, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    return 0;
}" SPR_HAVE_MMAP)

check_c_source_compiles("
#include <dirent.h>
#include <sys/types.h>
int main(void) {
    DIR *dir = opendir(\"/\");
    struct dirent *dirent = readdir(dir);
    dirent->d_type;
    return 0;
}" SPR_HAVE_D_TYPE)

check_c_source_compiles("
#include <unistd.h>
int main(void) {
    sysconf(_SC_PAGESIZE);
    return 0;
}" SPR_HAVE_SC_PAGESIZE)

check_c_source_compiles("
#include <unistd.h>
int main(void) {
    sysconf(_SC_NPROCESSORS_ONLN);
    return 0;
}" SPR_HAVE_SC_NPROC)

set(CMAKE_REQUIRED_LINK_OPTIONS -lpthread)
check_c_source_compiles("
#include <semaphore.h>
int main(void) {
    sem_t s;
    sem_init(&s, 0, 1);
    return 0;
}" SPR_HAVE_POSIX_SEM)

check_c_source_compiles("
#include <dispatch/dispatch.h>
int main(void) {
    dispatch_semaphore_create(1);
    return 0;
}" SPR_HAVE_GCD_SEM)

configure_file(
    "cmake/spr_config.h.in"
    "${PROJECT_BINARY_DIR}/spr_config.h"
)
