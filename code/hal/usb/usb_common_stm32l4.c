// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include <stdint.h>
#include <string.h>
#include "usb_common.h"
#include "private/usb_priv.h"
#include "stm32l4xx.h"

static usbd_device s_usb;
static uint8_t s_tx_buff[0x200];

static struct usb_cdc_line_coding cdc_line = {
    .dwDTERate          = 57600,
    .bCharFormat        = USB_CDC_1_STOP_BITS,
    .bParityType        = USB_CDC_NO_PARITY,
    .bDataBits          = 8,
};

static usbd_respond cdc_getdesc (usbd_ctlreq *req, void **address, uint16_t *length) {
    const uint8_t dtype = req->wValue >> 8;
    const uint8_t dnumber = req->wValue & 0xFF;
    const void* desc;
    uint16_t len = 0;
    switch (dtype) {
    case USB_DTYPE_DEVICE:
        desc = &device_desc;
        break;
    case USB_DTYPE_CONFIGURATION:
        desc = &config_desc;
        len = sizeof(config_desc);
        break;
    case USB_DTYPE_STRING:
        if (dnumber < 3) {
            desc = dtable[dnumber];
        } else {
            return usbd_fail;
        }
        break;
    default:
        return usbd_fail;
    }
    if (len == 0) {
        len = ((struct usb_header_descriptor*)desc)->bLength;
    }
    *address = (void*)desc;
    *length = len;
    return usbd_ack;
}

static usbd_respond cdc_control(usbd_device *dev, usbd_ctlreq *req, usbd_rqc_callback *callback) {
    if (((USB_REQ_RECIPIENT | USB_REQ_TYPE) & req->bmRequestType) == (USB_REQ_INTERFACE | USB_REQ_CLASS)
        && req->wIndex == 0 ) {
        switch (req->bRequest) {
        case USB_CDC_SET_CONTROL_LINE_STATE:
            return usbd_ack;
        case USB_CDC_SET_LINE_CODING:
            memcpy(&cdc_line, req->data, sizeof(cdc_line));
            return usbd_ack;
        case USB_CDC_GET_LINE_CODING:
            dev->status.data_ptr = &cdc_line;
            dev->status.data_count = sizeof(cdc_line);
            return usbd_ack;
        default:
            return usbd_fail;
        }
    }
    return usbd_fail;
}

static void cdc_rxonly (usbd_device *dev, uint8_t event, uint8_t ep) {
  usbd_ep_read(dev, ep, s_tx_buff, CDC_DATA_SZ);
}

static void cdc_txonly(usbd_device *dev, uint8_t event, uint8_t ep) {
  uint8_t _t = dev->driver->frame_no();
  memset(s_tx_buff, _t, CDC_DATA_SZ);
  usbd_ep_write(dev, ep, s_tx_buff, CDC_DATA_SZ);
}

static void cdc_rxtx(usbd_device *dev, uint8_t event, uint8_t ep)
{
  if (event == usbd_evt_eptx) {
    cdc_txonly(dev, event, ep);
  } else {
    cdc_rxonly(dev, event, ep);
  }
}

static usbd_respond cdc_setconf (usbd_device *dev, uint8_t cfg)
{
  switch (cfg)
  {
  case 0:
    /* deconfiguring device */
    usbd_ep_deconfig(dev, CDC_NTF_EP);
    usbd_ep_deconfig(dev, CDC_TXD_EP);
    usbd_ep_deconfig(dev, CDC_RXD_EP);
    usbd_reg_endpoint(dev, CDC_RXD_EP, 0);
    usbd_reg_endpoint(dev, CDC_TXD_EP, 0);
    return usbd_ack;
  case 1:
    /* configuring device */
    usbd_ep_config(dev, CDC_RXD_EP, USB_EPTYPE_BULK /*| USB_EPTYPE_DBLBUF*/, CDC_DATA_SZ);
    usbd_ep_config(dev, CDC_TXD_EP, USB_EPTYPE_BULK /*| USB_EPTYPE_DBLBUF*/, CDC_DATA_SZ);
    usbd_ep_config(dev, CDC_NTF_EP, USB_EPTYPE_INTERRUPT, CDC_NTF_SZ);
    usbd_reg_endpoint(dev, CDC_RXD_EP, cdc_rxtx);
    usbd_reg_endpoint(dev, CDC_TXD_EP, cdc_rxtx);
    usbd_ep_write(dev, CDC_TXD_EP, 0, 0);
    return usbd_ack;
  default:
    return usbd_fail;
  }
}

void OTG_FS_IRQHandler(void)
{
  usbd_poll(&s_usb);
}

void usb_init(void)
{
  uint32_t ubuf[0x20] = {0};
  usbd_init(&s_usb, &usbd_hw, CDC_EP0_SIZE, ubuf, sizeof(ubuf));

  usbd_reg_config(&s_usb, cdc_setconf);
  usbd_reg_control(&s_usb, cdc_control);
  usbd_reg_descr(&s_usb, cdc_getdesc);

  NVIC_EnableIRQ(OTG_FS_IRQn);

  usbd_enable(&s_usb, true);
}

int32_t usb_vcp_write(char* data, size_t length)
{
  static uint8_t connection_type = usbd_lane_dsc;

  // Host needs to support TX
  if (!((connection_type == usbd_lane_cdp) || (connection_type == usbd_lane_sdp)))
  {
    connection_type = usbd_connect(&s_usb, true);
  }

  if ((connection_type == usbd_lane_cdp) || (connection_type == usbd_lane_sdp))
  {
    return usbd_ep_write(&s_usb, CDC_TXD_EP, data, (uint16_t)length);
  }

  return -1;
}
