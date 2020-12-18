/*
 * USB_MSC.c
 *
 *  Created on: 17.12.2020
 *      Author: eike
 */

#include "USB_MSC.h"
#include "Flash.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/msc.h>
#include <libopencm3/usb/dwc/otg_fs.h>

#include <stdlib.h>

static usbd_device *msc_dev;
/* Buffer to be used for control requests. */
static uint8_t usbd_control_buffer[128];

static const struct usb_device_descriptor dev_descr = {
	.bLength = USB_DT_DEVICE_SIZE,
	.bDescriptorType = USB_DT_DEVICE,
	.bcdUSB = 0x0110,
	.bDeviceClass = 0,
	.bDeviceSubClass = 0,
	.bDeviceProtocol = 0,
	.bMaxPacketSize0 = 64,
	.idVendor = 0x0483,
	.idProduct = 0x5741,
	.bcdDevice = 0x0200,
	.iManufacturer = 1,
	.iProduct = 2,
	.iSerialNumber = 3,
	.bNumConfigurations = 1,
};

static const struct usb_endpoint_descriptor msc_endp[] = {{
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,
	.bEndpointAddress = 0x01,
	.bmAttributes = USB_ENDPOINT_ATTR_BULK,
	.wMaxPacketSize = 64,
	.bInterval = 0,
}, {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,
	.bEndpointAddress = 0x82,
	.bmAttributes = USB_ENDPOINT_ATTR_BULK,
	.wMaxPacketSize = 64,
	.bInterval = 0,
}};

static const struct usb_interface_descriptor msc_iface[] = {{
	.bLength = USB_DT_INTERFACE_SIZE,
	.bDescriptorType = USB_DT_INTERFACE,
	.bInterfaceNumber = 0,
	.bAlternateSetting = 0,
	.bNumEndpoints = 2,
	.bInterfaceClass = USB_CLASS_MSC,
	.bInterfaceSubClass = USB_MSC_SUBCLASS_SCSI,
	.bInterfaceProtocol = USB_MSC_PROTOCOL_BBB,
	.iInterface = 0,
	.endpoint = msc_endp,
	.extra = NULL,
	.extralen = 0
}};

static const struct usb_interface ifaces[] = {{
	.num_altsetting = 1,
	.altsetting = msc_iface,
}};

static const struct usb_config_descriptor config_descr = {
	.bLength = USB_DT_CONFIGURATION_SIZE,
	.bDescriptorType = USB_DT_CONFIGURATION,
	.wTotalLength = 0,
	.bNumInterfaces = 1,
	.bConfigurationValue = 1,
	.iConfiguration = 0,
	.bmAttributes = 0x80,
	.bMaxPower = 0x32,
	.interface = ifaces,
};

static const char *usb_strings[] = {
	"Black Sphere Technologies",
	"MSC Demo",
	"DEMO",
};

extern int USB_MSC_Flash_read(uint32_t lba, uint8_t *copy_to) {
	if(lba == 128) {
		asm volatile("nop");
	}
	Flash_StartRead(lba*512);
	for(int i=0; i<512; i++) {
		*(copy_to++) = Flash_Read();
	}
	Flash_StopRead();
}

extern int USB_MSC_Flash_write(uint32_t lba, const uint8_t *copy_from) {
	if(lba == 128) {
		asm volatile("nop");
	}
	if((lba & 0x07) == 0) {
		Flash_Erase4k(lba*512);
	}
	Flash_StartProgramPage(lba*512);
	for(int i = 0; i < 256; i++) {
		Flash_ProgramPage(*(copy_from++));
	}
	Flash_StopProgramPage();

	Flash_StartProgramPage(lba*512+256);
	for(int i = 0; i < 256; i++) {
		Flash_ProgramPage(*(copy_from++));
	}
	Flash_StopProgramPage();
}

extern int USB_MSC_Flash_blocks(void) {
	return 8192;
}

void USB_MSC_Init(void) {
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_OTGFS);

	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO11 | GPIO12);
	gpio_set_af(GPIOA, GPIO_AF10, GPIO11 | GPIO12);

	msc_dev = usbd_init(&otgfs_usb_driver, &dev_descr, &config_descr,
				usb_strings, 3,
				usbd_control_buffer, sizeof(usbd_control_buffer));

	OTG_FS_GCCFG |= OTG_GCCFG_NOVBUSSENS;

	usb_msc_init(msc_dev, 0x82, 64, 0x01, 64, "VendorID", "ProductID",
		"0.00", USB_MSC_Flash_blocks(), USB_MSC_Flash_read, USB_MSC_Flash_write);
}

void USB_MSC_Poll(void) {
	usbd_poll(msc_dev);
}
