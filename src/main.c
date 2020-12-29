#include <hw/Flash.h>
#include <hw/Serial.h>
#include <hw/Keypad.h>
#include <hw/led.h>
#include <hw/AudioPWM.h>
#include <logic/Soundboard.h>
#include <hw/clock.h>
#include <hw/USB_MSC.h>
#include <fatfs/ff.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/systick.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>


unsigned char short_beep_opus[] = {
	0x4f, 0x67, 0x67, 0x53, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xa7, 0x62, 0xd3, 0xb9, 0x00, 0x00, 0x00, 0x00, 0x89, 0xe0,
	0x22, 0x2a, 0x01, 0x13, 0x4f, 0x70, 0x75, 0x73, 0x48, 0x65, 0x61, 0x64,
	0x01, 0x01, 0x38, 0x01, 0x80, 0xbb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4f,
	0x67, 0x67, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xa7, 0x62, 0xd3, 0xb9, 0x01, 0x00, 0x00, 0x00, 0x31, 0x75, 0x7d,
	0xbe, 0x01, 0x3f, 0x4f, 0x70, 0x75, 0x73, 0x54, 0x61, 0x67, 0x73, 0x0d,
	0x00, 0x00, 0x00, 0x4c, 0x61, 0x76, 0x66, 0x35, 0x37, 0x2e, 0x38, 0x33,
	0x2e, 0x31, 0x30, 0x30, 0x01, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00,
	0x65, 0x6e, 0x63, 0x6f, 0x64, 0x65, 0x72, 0x3d, 0x4c, 0x61, 0x76, 0x63,
	0x35, 0x37, 0x2e, 0x31, 0x30, 0x37, 0x2e, 0x31, 0x30, 0x30, 0x20, 0x6c,
	0x69, 0x62, 0x6f, 0x70, 0x75, 0x73, 0x4f, 0x67, 0x67, 0x53, 0x00, 0x04,
	0xf8, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa7, 0x62, 0xd3, 0xb9,
	0x02, 0x00, 0x00, 0x00, 0x9d, 0x30, 0xbb, 0x91, 0x06, 0x2f, 0x1f, 0x23,
	0x25, 0x21, 0x1f, 0x28, 0x84, 0x27, 0x8f, 0x86, 0x32, 0x68, 0xc4, 0xa0,
	0x00, 0x13, 0x62, 0x86, 0x64, 0xe4, 0x98, 0x3f, 0x38, 0x80, 0x5f, 0x36,
	0xb4, 0x41, 0x72, 0xfc, 0x07, 0x79, 0xf9, 0x9f, 0xbf, 0xbd, 0xfd, 0xe0,
	0xb6, 0xa6, 0xa6, 0x5e, 0xb2, 0xed, 0x98, 0xb2, 0x80, 0xe2, 0x23, 0x98,
	0x7e, 0x60, 0x28, 0xb2, 0xfc, 0xda, 0xbc, 0x60, 0xdd, 0x77, 0x96, 0xbf,
	0x4d, 0x3e, 0x94, 0xf8, 0x4c, 0xac, 0xf8, 0x00, 0x9d, 0x31, 0x94, 0xa0,
	0xdf, 0x6b, 0xaa, 0x02, 0x1b, 0x6d, 0x68, 0x7e, 0x40, 0x28, 0xa9, 0x80,
	0x26, 0xbe, 0x77, 0xd4, 0x11, 0xab, 0xa3, 0xe0, 0xd5, 0x91, 0x63, 0xfc,
	0x44, 0x7e, 0x70, 0xc5, 0xaa, 0x5d, 0xd8, 0xdc, 0xfc, 0x05, 0x13, 0x72,
	0x7e, 0x09, 0x3b, 0x3b, 0x86, 0x1b, 0x39, 0x80, 0x28, 0xa9, 0x80, 0x26,
	0xbe, 0x77, 0xd4, 0x11, 0xab, 0xa3, 0xa0, 0xe3, 0x3b, 0x11, 0x26, 0xba,
	0x48, 0x56, 0x58, 0x88, 0xf7, 0xc5, 0xa4, 0x93, 0x31, 0x87, 0xd2, 0xf5,
	0x02, 0x82, 0xe2, 0xe6, 0x3f, 0x28, 0x49, 0xab, 0xc3, 0x28, 0xa9, 0x80,
	0x26, 0xbe, 0x77, 0xd4, 0x11, 0xab, 0xa8, 0xba, 0xb6, 0xf0, 0x98, 0x31,
	0x7e, 0xe9, 0xdd, 0xfd, 0xb0, 0xb0, 0x3d, 0x75, 0xd6, 0xf6, 0x12, 0x6d,
	0xc3, 0x20, 0x6e, 0x6d, 0x45, 0x96, 0x28, 0xb2, 0xbb, 0xb7, 0xe8, 0xd3,
	0x0c, 0x88, 0xd0, 0x98, 0x14, 0xbc, 0xb6, 0x2c, 0x03, 0x0c, 0x20, 0x60,
	0xf4, 0x8b, 0x1a, 0x3f, 0x23, 0xc8, 0xc8, 0xd5, 0x4f, 0x7c, 0x00, 0xbb,
	0x92
};

unsigned char short_beep_hq_opus[] = {
	0x4f, 0x67, 0x67, 0x53, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x1a, 0x06, 0xc7, 0xb9, 0x00, 0x00, 0x00, 0x00, 0x5b, 0xaa,
	0x61, 0x2f, 0x01, 0x13, 0x4f, 0x70, 0x75, 0x73, 0x48, 0x65, 0x61, 0x64,
	0x01, 0x01, 0x38, 0x01, 0x80, 0xbb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4f,
	0x67, 0x67, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x1a, 0x06, 0xc7, 0xb9, 0x01, 0x00, 0x00, 0x00, 0x25, 0xd7, 0x58,
	0x9f, 0x01, 0x3f, 0x4f, 0x70, 0x75, 0x73, 0x54, 0x61, 0x67, 0x73, 0x0d,
	0x00, 0x00, 0x00, 0x4c, 0x61, 0x76, 0x66, 0x35, 0x37, 0x2e, 0x38, 0x33,
	0x2e, 0x31, 0x30, 0x30, 0x01, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00,
	0x65, 0x6e, 0x63, 0x6f, 0x64, 0x65, 0x72, 0x3d, 0x4c, 0x61, 0x76, 0x63,
	0x35, 0x37, 0x2e, 0x31, 0x30, 0x37, 0x2e, 0x31, 0x30, 0x30, 0x20, 0x6c,
	0x69, 0x62, 0x6f, 0x70, 0x75, 0x73, 0x4f, 0x67, 0x67, 0x53, 0x00, 0x00,
	0xc0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a, 0x06, 0xc7, 0xb9,
	0x02, 0x00, 0x00, 0x00, 0x1b, 0x60, 0xcb, 0x7f, 0x01, 0x9b, 0x78, 0x82,
	0xe2, 0x5d, 0x6c, 0x47, 0x24, 0xea, 0x03, 0xf4, 0x31, 0x3b, 0xf5, 0x98,
	0x6d, 0xe0, 0xfd, 0x91, 0xef, 0x84, 0x3d, 0xc4, 0x75, 0x44, 0xfa, 0x74,
	0xd9, 0xa6, 0x14, 0x5c, 0xe5, 0xb9, 0x1a, 0x71, 0xab, 0xcc, 0x1a, 0xd1,
	0x6c, 0xec, 0x08, 0xf2, 0xa0, 0xab, 0xfe, 0xe7, 0xad, 0x15, 0x9d, 0xa6,
	0x35, 0xe0, 0xb0, 0x3c, 0xfc, 0x8d, 0x1f, 0x34, 0x6e, 0x8a, 0x0e, 0xdd,
	0x86, 0xc5, 0x1a, 0x40, 0x0c, 0x96, 0x5e, 0xf2, 0x23, 0x87, 0xd0, 0xc4,
	0xc2, 0xf7, 0x4c, 0xc0, 0xa5, 0xff, 0x8d, 0x1c, 0x3b, 0xa5, 0xfc, 0xfc,
	0x16, 0xe2, 0x0c, 0x2c, 0x53, 0xa2, 0x3a, 0x6a, 0x89, 0xc3, 0xd6, 0xb8,
	0xb7, 0xb5, 0x60, 0x50, 0x20, 0x9e, 0x1f, 0x0f, 0xab, 0x13, 0x6c, 0x59,
	0xa1, 0x51, 0xd7, 0xaf, 0xb2, 0x45, 0xc0, 0x95, 0x8c, 0x5a, 0xc6, 0xda,
	0xc7, 0x28, 0x9e, 0x23, 0x11, 0xc4, 0xaf, 0xda, 0xa7, 0x1d, 0x0c, 0x37,
	0x29, 0x76, 0x2c, 0x75, 0xef, 0x80, 0xc4, 0x91, 0xc4, 0xd8, 0xa0, 0x23,
	0x5b, 0x78, 0x5e, 0x2b, 0x62, 0x0e, 0x1e, 0x15, 0xaa, 0x4f, 0x67, 0x67,
	0x53, 0x00, 0x00, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a,
	0x06, 0xc7, 0xb9, 0x03, 0x00, 0x00, 0x00, 0x50, 0x6a, 0xf2, 0x28, 0x01,
	0x5d, 0x78, 0xa8, 0xcc, 0x69, 0x8b, 0xed, 0x47, 0x88, 0x1d, 0x01, 0xd5,
	0xb8, 0x85, 0xf5, 0x81, 0xc1, 0x33, 0x54, 0xb3, 0x4f, 0xbe, 0x64, 0xad,
	0x31, 0xa4, 0xd5, 0x39, 0x4a, 0xc9, 0x19, 0x4c, 0x19, 0x1d, 0x48, 0x68,
	0x9f, 0x85, 0x87, 0x61, 0xaa, 0x48, 0xbc, 0x03, 0x64, 0x78, 0x81, 0x3b,
	0x0c, 0xd4, 0x72, 0x2e, 0x13, 0xbb, 0x93, 0xdf, 0xb4, 0xf9, 0x0a, 0x3d,
	0xac, 0x33, 0xb8, 0x43, 0xc4, 0x67, 0x4c, 0x74, 0x8c, 0xef, 0xcb, 0xb9,
	0xc0, 0x55, 0xed, 0x66, 0x89, 0x7e, 0x20, 0xe2, 0x43, 0x80, 0xd4, 0xb8,
	0x27, 0x1a, 0x22, 0x7b, 0xae, 0xb9, 0x8f, 0xfd, 0x4f, 0x32, 0x4f, 0x67,
	0x67, 0x53, 0x00, 0x00, 0x40, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x1a, 0x06, 0xc7, 0xb9, 0x04, 0x00, 0x00, 0x00, 0xd3, 0x5d, 0xed, 0xc6,
	0x01, 0x63, 0x78, 0xa1, 0x42, 0xec, 0x09, 0xb4, 0x1f, 0xff, 0x2d, 0x73,
	0xd9, 0x35, 0x32, 0x53, 0xbb, 0x1c, 0x68, 0x70, 0x91, 0xf1, 0x1e, 0xa9,
	0x1b, 0x18, 0xe4, 0x1a, 0x9b, 0x3b, 0x5d, 0xd2, 0xb9, 0x9a, 0x99, 0xd2,
	0x48, 0x1b, 0x38, 0x8c, 0x0a, 0xb2, 0xdc, 0x0d, 0x84, 0x92, 0x21, 0xaa,
	0x0f, 0x5b, 0xc0, 0xc7, 0x65, 0x11, 0x84, 0x65, 0xed, 0x56, 0xc7, 0x42,
	0x9b, 0xc4, 0xf9, 0x54, 0xec, 0xf0, 0xa0, 0x25, 0x1c, 0xae, 0x21, 0x06,
	0x18, 0x02, 0xbd, 0x2e, 0xe6, 0x8c, 0x43, 0x03, 0x9f, 0xe9, 0x38, 0x3c,
	0x68, 0x2e, 0x7d, 0xf2, 0x17, 0x82, 0xec, 0xc4, 0x1d, 0x3e, 0xc9, 0xb6,
	0x37, 0xbf, 0xb8, 0xd9, 0x97, 0x4f, 0x67, 0x67, 0x53, 0x00, 0x00, 0x00,
	0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a, 0x06, 0xc7, 0xb9, 0x05,
	0x00, 0x00, 0x00, 0xb6, 0xdf, 0x69, 0x8c, 0x01, 0x63, 0x78, 0xa1, 0x42,
	0xec, 0x10, 0x59, 0xd2, 0x57, 0x2d, 0x76, 0x1a, 0xac, 0x91, 0x3f, 0x3a,
	0xbb, 0xed, 0xc6, 0x9b, 0x43, 0x76, 0xe9, 0xf9, 0x71, 0x77, 0xba, 0x7b,
	0x19, 0x7a, 0xd3, 0x73, 0xf0, 0x0b, 0x5f, 0x4f, 0x1e, 0xff, 0x4e, 0x3b,
	0xc3, 0xdc, 0x78, 0x06, 0x3f, 0xfe, 0x20, 0xd6, 0x59, 0xe8, 0xd1, 0x77,
	0xda, 0x16, 0x55, 0x6c, 0x37, 0xe0, 0x65, 0x64, 0xe7, 0x99, 0x70, 0xad,
	0x7d, 0xb4, 0xde, 0x67, 0xa5, 0x48, 0xa5, 0x71, 0xf3, 0x1d, 0xaf, 0x86,
	0xa3, 0x58, 0x7c, 0xcb, 0xe8, 0xfa, 0xde, 0x8f, 0x32, 0x66, 0x18, 0x54,
	0xf5, 0x59, 0x1d, 0xe2, 0x60, 0x93, 0xcc, 0xea, 0x55, 0x02, 0x15, 0x17,
	0x4f, 0x67, 0x67, 0x53, 0x00, 0x00, 0xc0, 0x12, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x1a, 0x06, 0xc7, 0xb9, 0x06, 0x00, 0x00, 0x00, 0x4a, 0xb0,
	0xd2, 0x65, 0x01, 0x5e, 0x78, 0xa1, 0x42, 0xec, 0x09, 0xb4, 0x1f, 0xff,
	0x2d, 0x74, 0xd4, 0x3e, 0x4e, 0x84, 0x3b, 0x30, 0xda, 0xaa, 0x0b, 0xa2,
	0x05, 0x38, 0xfa, 0x3d, 0x58, 0xfe, 0x39, 0x33, 0x84, 0xc3, 0x0c, 0x45,
	0x17, 0xf7, 0x6c, 0xaa, 0xfb, 0x72, 0xe3, 0x55, 0x0c, 0x20, 0x8b, 0x1d,
	0xc9, 0x78, 0x03, 0x68, 0xe5, 0xe2, 0xaa, 0xe5, 0xf7, 0x0e, 0x43, 0xfd,
	0xa8, 0x25, 0x62, 0x8b, 0x08, 0xa1, 0x58, 0x99, 0xe5, 0x77, 0x0c, 0x31,
	0xea, 0x67, 0x62, 0x70, 0x47, 0xeb, 0xc8, 0x12, 0xeb, 0xad, 0xe5, 0x19,
	0xa2, 0x97, 0x07, 0x09, 0x5e, 0x13, 0x3b, 0x34, 0x63, 0x21, 0x85, 0x8a,
	0x15, 0x5f, 0x4f, 0x67, 0x67, 0x53, 0x00, 0x04, 0xf8, 0x13, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x1a, 0x06, 0xc7, 0xb9, 0x07, 0x00, 0x00, 0x00,
	0xe3, 0xfb, 0x05, 0x34, 0x01, 0x5f, 0x78, 0xa6, 0xd8, 0xdd, 0xcb, 0xaf,
	0x6b, 0xf1, 0xc8, 0xc6, 0x7f, 0x8b, 0xa5, 0x7c, 0x3d, 0xd9, 0xf9, 0x85,
	0x5b, 0x00, 0xd1, 0x02, 0x7c, 0xbd, 0x4f, 0xcc, 0xf8, 0x8e, 0x1f, 0x67,
	0xb1, 0x6c, 0xfa, 0x3f, 0xfd, 0x69, 0xf1, 0x25, 0x88, 0x0a, 0xd1, 0x95,
	0x63, 0x1c, 0xa3, 0xd3, 0x54, 0xee, 0x79, 0x74, 0xd7, 0xd6, 0x2d, 0xe3,
	0x20, 0xb0, 0xba, 0xc7, 0x86, 0x94, 0xfe, 0xfe, 0x5d, 0xf4, 0x01, 0xf3,
	0xf5, 0x00, 0x0c, 0xd7, 0x37, 0x69, 0x17, 0xbe, 0x89, 0x9b, 0x2c, 0x24,
	0x45, 0x28, 0xf0, 0xf2, 0xcd, 0x77, 0xb8, 0x34, 0x14, 0xb5, 0x63, 0xf7,
	0x22, 0xc6, 0x3a, 0xf5, 0x22
	};

int ogg_pos;

#include <opus.h>

int ogg_read(uint8_t *buffer, int len) {
	if(ogg_pos + len > sizeof(short_beep_opus)) {
		len = sizeof(short_beep_opus) - ogg_pos;
	}

	memcpy(buffer, short_beep_opus+ogg_pos, len);
	ogg_pos += len;
	return len;
}


typedef struct {
	uint8_t magic_number[4];
	uint8_t version;
	uint8_t type;
	uint64_t granule_pos;
	uint32_t bitstream_serial;
	uint32_t page_sequence_number;
	uint32_t checksum;
	uint8_t page_segments;
} ogg_page_header_t;


bool bytescompare(const char *s1, uint8_t *s2, int len) {
	while(len-- > 0) {
		if (*(s1++) != *(s2++)) return false;
	}
	return true;
}

typedef enum {
	OGG_OK = 0,
	OGG_MAGICNUMBERINVALID,
	OGG_IDPAGEINVALID,
	OGG_BADCHANNELCOUNT,
	OGG_MISCINVALID,
	OGG_BADSAMPLINGRATE,
	OGG_COMMENTPAGEINVALID,
	OGG_EOS,
} Ogg_Status_t;

void OggOpus_DecodeHeader(uint8_t buffer[26], ogg_page_header_t *header) {
	header->magic_number[0] = buffer[0];
	header->magic_number[1] = buffer[1];
	header->magic_number[2] = buffer[2];
	header->magic_number[3] = buffer[3];
	header->version = buffer[4];
	header->type = buffer[5];
	header->granule_pos =
			((uint64_t)buffer[ 6] <<  0) |
			((uint64_t)buffer[ 7] <<  8) |
			((uint64_t)buffer[ 8] << 16) |
			((uint64_t)buffer[ 9] << 24) |
			((uint64_t)buffer[10] << 32) |
			((uint64_t)buffer[11] << 40) |
			((uint64_t)buffer[12] << 48) |
			((uint64_t)buffer[13] << 56);
	header->bitstream_serial =
			((uint32_t)buffer[14] <<  0) |
			((uint32_t)buffer[15] <<  8) |
			((uint32_t)buffer[16] << 16) |
			((uint32_t)buffer[17] << 24);
	header->page_sequence_number =
			((uint32_t)buffer[18] <<  0) |
			((uint32_t)buffer[19] <<  8) |
			((uint32_t)buffer[20] << 16) |
			((uint32_t)buffer[21] << 24);
	header->checksum =
			((uint32_t)buffer[22] <<  0) |
			((uint32_t)buffer[23] <<  8) |
			((uint32_t)buffer[24] << 16) |
			((uint32_t)buffer[25] << 24);
	header->page_segments = buffer[26];
}

int OggOpus_ReadHeader(ogg_page_header_t *header, uint8_t *segment_table) {
	uint8_t buffer[27];
	// TODO return OGG_EOS if no header readable
	ogg_read(buffer, 27);
	OggOpus_DecodeHeader(buffer, header);
	if(!bytescompare("OggS", header->magic_number, 4)) return OGG_MAGICNUMBERINVALID;

	ogg_read(segment_table, header->page_segments);

	return OGG_OK;
}

void OggOpus_ReadSegment(uint8_t *segment, uint8_t size) {
	ogg_read(segment, size);
}

int OggOpus_ParseFirstPages(int *preskip) {
	// rfc7845 defines two mandatory pages, ID and comment in the beginning

	ogg_page_header_t header;
	uint8_t segment_table[256];
	uint8_t segment[256];

	// ID page
	if(OggOpus_ReadHeader(&header, (uint8_t *)segment_table) == OGG_MAGICNUMBERINVALID) {
		return OGG_MAGICNUMBERINVALID;
	}

	if(header.granule_pos != 0) return OGG_MISCINVALID;
	if(header.page_segments != 1) return OGG_MISCINVALID;

	OggOpus_ReadSegment((uint8_t *)segment, segment_table[0]);

	// Check for ID-Header
	if(!bytescompare("OpusHead", segment, 8)) return OGG_BADCHANNELCOUNT;
	// Check mono
	if(segment[9] != 1) return OGG_BADCHANNELCOUNT;
	// get preskip (segment[10:11], little endian)
	*preskip = segment[10] | (segment[11] << 8);
	// TODO check samplingrate? (segment[12:15], https://tools.ietf.org/html/rfc7845#page-14)

	// Comment page
	if(OggOpus_ReadHeader(&header, (uint8_t *)segment_table) == OGG_MAGICNUMBERINVALID) {
		return OGG_MAGICNUMBERINVALID;
	}

	if(header.granule_pos != 0) return OGG_MISCINVALID;
	if(header.page_segments != 1) return OGG_MISCINVALID;

	OggOpus_ReadSegment((uint8_t *)segment, segment_table[0]);

	if(!bytescompare("OpusTags", segment, 8)) return OGG_COMMENTPAGEINVALID;

	return OGG_OK;
}

static int OggOpus_CurrentSegment;
static uint8_t OggOpus_SegmentTable[255];
static uint8_t OggOpus_SegmentTableSize;

int OggOpus_ReadPacket(uint8_t *packet, int *size) {
	// It is assumed that each Ogg segment contains EXACTLY one opus packet.
	// We exploit that behaviour of most muxers for low bit rates.
	// Don't do this at home, kids! Use a propper repacketizer!

	if(OggOpus_CurrentSegment >= OggOpus_SegmentTableSize) {
		// TODO load new page
		ogg_page_header_t header;
		int status = OggOpus_ReadHeader(&header, OggOpus_SegmentTable);
		OggOpus_CurrentSegment = 0;
		OggOpus_SegmentTableSize = header.page_segments;

		// TODO return OGG_EOS if no next page available
		if(status == OGG_EOS) {
			return OGG_EOS;
		}
	}

	*size = OggOpus_SegmentTable[OggOpus_CurrentSegment];
	OggOpus_ReadSegment(packet, *size);

	OggOpus_CurrentSegment++;
	return OGG_OK;
}

uint8_t Opus_Decode_Space[17776];

int main()
{
	Clock_Init();
	Led_Init();
	Flash_Init();
	OpusDecoder *dec;
	volatile int size = opus_decoder_get_size(1);
	dec = (OpusDecoder *)Opus_Decode_Space;
	volatile int error = opus_decoder_init(dec, 48000, 1);

	int preskip;
	int ogg_status = OggOpus_ParseFirstPages(&preskip);
	if(ogg_status != OGG_OK) {
		while(1) {
			asm volatile("nop");
		}
	}

	while(1) {
		static uint8_t opus_buffer[256];
		int packetSize;
		OggOpus_ReadPacket((uint8_t *)opus_buffer, &packetSize);
		static int16_t samples[960];
		volatile int frames = opus_decode(dec, opus_buffer, packetSize, samples, 960, 0);
		asm volatile("nop");
	}

	while(1);
}

//int main()
//{
//	Clock_Init();
//	Led_Init();
//	Flash_Init();
//	USB_MSC_Init();
//	AudioPWM_Init();
//	Keypad_Init();
//
//	for(int i = 0; i<1000000; i++);
//
//	// Run soundboard
//	Soundboard_Run();
//
//	while(1) {
//		asm volatile("nop");
//	}
//	return 0;
//}
//{
//	Clock_Init();
//	Led_Init();
//	AudioPWM_Init();
//
//	OpusDecoder *dec;
//	volatile int size = opus_decoder_get_size(1);
//	dec = malloc(size);
//	volatile int error = opus_decoder_init(dec, 48000, 1);
//
//	static int16_t samples[4096];
//	volatile int start = systick_get_value();
//	volatile int frames = opus_decode(dec, short_beep_opus, sizeof(short_beep_opus), samples, 4096, 0);
//	volatile int end = systick_get_value();
//
//	while(1);
//}
