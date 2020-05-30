set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR AVR)

set(TOOLCHAIN_PREFIX avr-)

set(COMMON_FLAGS "-mmcu=atmega328p -Wall -Werror -mcall-prologues")

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc CACHE INTERNAL "C Compiler")
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER} CACHE INTERNAL "ASM Compiler")
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++ CACHE INTERNAL "C++ Compiler")
set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy CACHE INTERNAL "Objcopy tool")
set(CMAKE_SIZE_UTIL ${TOOLCHAIN_PREFIX}size CACHE INTERNAL "Size tool")

set(CMAKE_C_FLAGS   "${COMMON_FLAGS} -std=c99 " CACHE INTERNAL "C Compiler options")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -std=c++98 " CACHE INTERNAL "C++ Compiler options")
set(CMAKE_ASM_FLAGS "${COMMON_FLAGS} -x assembler-with-cpp " CACHE INTERNAL "ASM Compiler options")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,-gc-sections -Wl,-relax -Wl,-Map=${CMAKE_PROJECT_NAME}.map" CACHE INTERNAL "Linker options")

set(CMAKE_C_FLAGS_DEBUG "-O0 -g" CACHE INTERNAL "C Compiler options for debug build type")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g" CACHE INTERNAL "C++ Compiler options for debug build type")
set(CMAKE_ASM_FLAGS_DEBUG "-g" CACHE INTERNAL "ASM Compiler options for debug build type")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "" CACHE INTERNAL "Linker options for debug build type")

set(CMAKE_C_FLAGS_RELEASE "-O1 -g" CACHE INTERNAL "C Compiler options for release build type")
set(CMAKE_CXX_FLAGS_RELEASE "-O1 -g" CACHE INTERNAL "C++ Compiler options for release build type")
set(CMAKE_ASM_FLAGS_RELEASE "-g" CACHE INTERNAL "ASM Compiler options for release build type")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "" CACHE INTERNAL "Linker options for release build type")
