// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef INTERRUPT_HANDLERS_H
#define INTERRUPT_HANDLERS_H

/**
 * @brief Initialise hardware event interrupts
 * @note Switches will not function correctly until this function is called, because debouncing
 * relies on a timer event that requires initialisation.
 */
void interrupt_init(void);

#endif /* INTERRUPT_HANDLERS_H */
