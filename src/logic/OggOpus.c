/*
 * OggOpus.c
 *
 *  Created on: 30.12.2020
 *      Author: eike
 */

#include "OggOpus.h"

#include "Sound.h"

#include <fatfs/ff.h>
#include <opus.h>
#include <stdint.h>
#include <stdbool.h>

#define OGGOPUS_OPUSDECODERSIZE 17776

static int OggOpus_CurrentSegment;
static uint8_t OggOpus_SegmentTable[255];
static uint8_t OggOpus_SegmentTableSize;

static OpusDecoder *OggOpus_OpusDecoder;
static uint8_t OggOpus_Decode_Space[OGGOPUS_OPUSDECODERSIZE];

static int16_t OggOpus_CurrentBuffer[4096];
static int OggOpus_CurrentBufferPos;
static int OggOpus_CurrentBufferLength;

void OggOpus_Init() {
	OggOpus_OpusDecoder = (OpusDecoder *)OggOpus_Decode_Space;

	// The decoder space is pre-allocated, but since the actual size
	// is calculated dynamically, it might have to be adjusted for
	// future versions of opus
	if(opus_decoder_get_size(1) != OGGOPUS_OPUSDECODERSIZE) {
		// Check the Opus decoder size and adjust to match opus_decoder_get_size(1)
		while(1);
	}
	int status = opus_decoder_init(OggOpus_OpusDecoder, 48000, 1);
}


bool bytescompare(const char *s1, uint8_t *s2, int len) {
	while(len-- > 0) {
		if (*(s1++) != *(s2++)) return false;
	}
	return true;
}


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

Ogg_Status_t OggOpus_ReadHeader(ogg_page_header_t *header, uint8_t *segment_table) {
	uint8_t buffer[27];
	uint32_t bytesread = Sound_ReadCurrentFile(buffer, 27);

	if(bytesread != 27) {
		// return OGG_EOS if no header readable
		return OGG_EOS;
	}

	OggOpus_DecodeHeader(buffer, header);
	if(!bytescompare("OggS", header->magic_number, 4)) return OGG_MAGICNUMBERINVALID;

	Sound_ReadCurrentFile(segment_table, header->page_segments);

	return OGG_OK;
}

void OggOpus_ReadSegment(uint8_t *segment, uint8_t size) {
	Sound_ReadCurrentFile(segment, size);
}

Ogg_Status_t OggOpus_ParseFirstPages(int *preskip) {
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

Ogg_Status_t OggOpus_ReadPacket(uint8_t *packet, int *size) {
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

Ogg_Status_t OggOpus_NextPacket() {
	uint8_t opus_buffer[255];
	int size;
	Ogg_Status_t status = OggOpus_ReadPacket((uint8_t *)opus_buffer, &size);
	if(status != OGG_OK) return status;
	OggOpus_CurrentBufferLength = opus_decode(OggOpus_OpusDecoder, opus_buffer, size, OggOpus_CurrentBuffer, 4096, 0);
	OggOpus_CurrentBufferPos = 0;
	return OGG_OK;
}

void OggOpus_StartSound() {
	int preskip;
	int status = OggOpus_ParseFirstPages(&preskip);

	if(status != OGG_OK)
	{
		return;
	}

	while(preskip-- > 0) {
		int16_t drop_sample;
		OggOpus_GetSample(&drop_sample);
	}
}

bool OggOpus_GetSample(int16_t *sample) {
	if(OggOpus_CurrentBufferPos >= OggOpus_CurrentBufferLength) {
		Ogg_Status_t status = OggOpus_NextPacket();
		if(status != OGG_OK) {
			return false;
		}
	}
	sample = OggOpus_CurrentBuffer[OggOpus_CurrentBufferPos];
	OggOpus_CurrentBufferPos++;

	return true;
}
