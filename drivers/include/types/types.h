#ifndef TYPES_H
#define TYPES_H

#if defined(BOARD_MK1)
#include "types/types_avr.h"
#elif defined(BOARD_SIMULATED) || defined(BOARD_MOCK)
#include "types/types_x86.h"
#endif

#if defined(BOARD_MOCK)
#define TESTABLE_PREFIX virtual
#define TESTABLE_SUFFIX = 0
#define TESTABLE_DESTRUCTOR(name) virtual ~name() = default
#else
#define TESTABLE_PREFIX
#define TESTABLE_SUFFIX
#define TESTABLE_DESTRUCTOR(name)
#endif


enum class Error {
  NONE,
  SENSORS_NOT_READY
};

#endif /* TYPES_H */
