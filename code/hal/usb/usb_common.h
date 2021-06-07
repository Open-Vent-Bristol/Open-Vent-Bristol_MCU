// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef USB_COMMON_H
#define USB_COMMON_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Initialise the USB peripheral and the virtual COM port (VCP) endpoint
 */
void usb_init(void);

/**
 * @brief Write ASCII encoded text to the host computer via the VCP endpoint
 *
 * @param data      Pointer to character data
 * @param length    Byte length of write
 * @return int32_t  Bytes written, or -1 if unsuccessful, or no host
 */
int32_t usb_vcp_write(char* data, size_t length);

#endif /* USB_COMMON_H */
