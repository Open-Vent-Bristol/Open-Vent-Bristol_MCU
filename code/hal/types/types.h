#ifndef TYPES_H
#define TYPES_H

#ifdef BOARD_MK1
#include "types/types_avr.h"
#elif defined(BOARD_MK2)
#include "types/types_arm.h"
#endif

#endif /* TYPES_H */
