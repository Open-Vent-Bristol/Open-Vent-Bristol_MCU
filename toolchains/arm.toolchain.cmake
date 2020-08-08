set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

set(TOOLCHAIN_PREFIX arm-none-eabi-)

set(COMMON_FLAGS "-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -fno-builtin -Werror -Wall -ffunction-sections -fdata-sections -fomit-frame-pointer -mabi=aapcs")

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc CACHE INTERNAL "C Compiler")
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER} CACHE INTERNAL "ASM Compiler")
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++ CACHE INTERNAL "C++ Compiler")
set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy CACHE INTERNAL "Objcopy tool")
set(CMAKE_SIZE_UTIL ${TOOLCHAIN_PREFIX}size CACHE INTERNAL "Size tool")

set(CMAKE_C_FLAGS   "${COMMON_FLAGS} -std=c99 " CACHE INTERNAL "C Compiler options")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -std=c++14 " CACHE INTERNAL "C++ Compiler options")
set(CMAKE_ASM_FLAGS "${COMMON_FLAGS} -x assembler-with-cpp " CACHE INTERNAL "ASM Compiler options")
set(CMAKE_EXE_LINKER_FLAGS
    "-mcpu=cortex-m4 -T${CMAKE_CURRENT_LIST_DIR}/STM32L476RGTX_FLASH.ld --specs=nosys.specs -Wl,-Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections -static --specs=nano.specs -Wl,--start-group -lc -lm -Wl,--end-group"
    CACHE INTERNAL "Linker options")

set(CMAKE_C_FLAGS_DEBUG "-Og -g3" CACHE INTERNAL "C Compiler options for debug build type")
set(CMAKE_CXX_FLAGS_DEBUG "-Og -g3" CACHE INTERNAL "C++ Compiler options for debug build type")
set(CMAKE_ASM_FLAGS_DEBUG "-g3" CACHE INTERNAL "ASM Compiler options for debug build type")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "" CACHE INTERNAL "Linker options for debug build type")

set(CMAKE_C_FLAGS_RELEASE "-O1" CACHE INTERNAL "C Compiler options for release build type")
set(CMAKE_CXX_FLAGS_RELEASE "-O1" CACHE INTERNAL "C++ Compiler options for release build type")
set(CMAKE_ASM_FLAGS_RELEASE "" CACHE INTERNAL "ASM Compiler options for release build type")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "" CACHE INTERNAL "Linker options for release build type")
