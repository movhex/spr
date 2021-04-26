# Simplified Portable Runtime Library (SPR)

SPR is a library provides cross-platform C/C++ API for platform-specific functionality (such as work with files and threads).

SPR allows you to write programs that are portable to different platforms. In order to run a program on a different platform, it is enough to simply recompile the program using a compiler for a specific platform.

With SPR and no direct calls to OS system calls, fewer changes are required when porting to new platforms.

The functions of accessing the operating system are scattered among the program components and without using SPR, a large amount of source code would have to be modified during porting.

Porting the library itself is easy because all the platform-specific code is concentrated in one place. The migration process is about implementing all SPR functionality.

SPR is fully implemented in C and is compatible with the C99 standard.

The library covers next platform-independent functionality:
* Memory allocation
* Region-based memory management system
* File I/O
* Threads
* Mutexes
* Semaphores
* Network sockets
* Dynamic shared objects
* Strings, lists and arrays
* System error codes


## Currently supported platforms

* GNU/Linux
* FreeBSD 12
* macOS
* Windows 7/8/10

## Planned platforms

* Solaris

The library is free and open-source software distributed under BSD 2-clause license.
