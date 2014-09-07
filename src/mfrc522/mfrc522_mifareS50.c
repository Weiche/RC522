/*
 * mfrc522_mifareS50.c
 *
 *  Created on: 2014年9月7日
 *      Author: gitetsu
 */

#include "mfrc522.h"
#include <stdint.h>
#include <error.h>

#define S50_MAX_BYTE	(16*2 + 16*3*15)
typedef struct {
	uint8_t card_ID[4];
	uint8_t auth_key[6];
	uint8_t aes_key[16];
	uint8_t sector_buffer[4][16];
	int timeout;
	PICC_TYPE_t card_type;
	enum{
		IDLE,
		OPENED,
		UNKNOWN
	} status;
	void* next;
} Tag;
#define O_MATCH	(1<<0)
/* Function to open card with the protocol */
void *RFID_Protocol_open(Tag *ptag,int flags) {
	uint8_t CardID[5];
	int ret = 0;
	ret = MFRC522_Check(CardID);
	if (ret != MI_OK) {
		goto error;
	}
	if ( (flags & O_MATCH) && ( memcmp(ptag->card_ID, CardID, 4) != 0)) {
			goto error;
	}
	/* match any card */
	ret = MFRC522_SelectTag(CardID);
	if (MFRC522_ParseType(ret) != PICC_TYPE_MIFARE_1K) {
		goto error;
	}
	/* care selected */
	memcpy(ptag->card_ID,CardID,4);
	ptag->card_type = PICC_TYPE_MIFARE_1K;
	ptag->status = OPENED;
	return ptag;

	error: return 0;
}
void RFID_Protocol_close(Tag *pcard) {
	return;
}
size_t RFID_Protocol_read(void *ptr, size_t size,
		     size_t offset, Tag *ptag){
	return 0;
}
int RFID_Protocol_dump(Tag *ptag){
	uint8_t buffer[1024];
	size_t size;
	if( ptag->status != OPENED){
		return -1;
	}
	size = RFID_Protocol_read(buffer, S50_MAX_BYTE , 0 ,ptag);
	{
		dump(buffer,size);
		return 0;
	}

}

