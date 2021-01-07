/*
 * OggOpus.h
 *
 *  Created on: 30.12.2020
 *      Author: eike
 */

#ifndef LOGIC_OGGOPUS_H_
#define LOGIC_OGGOPUS_H_

#include <stdint.h>
#include <stdbool.h>

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

void OggOpus_Init(void);
void OggOpus_StartSound(void);
bool OggOpus_GetSample(int16_t *sample);

#endif /* LOGIC_OGGOPUS_H_ */
