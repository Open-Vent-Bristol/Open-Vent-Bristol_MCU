#ifndef BOARD_H
#define BOARD_H

#if defined(UTEST_MODE)
#include "board_unittest.h"
#elif defined(BOARD_MK1)
#include "board/board_mk1.h"
#elif defined(BOARD_MK2)
#include "board/board_mk2.h"
#endif

#endif /* BOARD_H */
