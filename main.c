/*
 * main.c
 *
 *  Created on: 2014��9��2��
 *      Author: Administrator
 */

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "mfrc522.h"
#define WIDTH   (18)

const void *dump(const void *addr, size_t bytes) {
	const unsigned char *p = addr;
	char text[WIDTH + 1];
	unsigned i = 0;

	while (i < bytes) {
		if ((i % WIDTH) == 0) {
			printf("%6d: ", i);

			memset(text, '\0', sizeof(text));
		}

		printf("%02x ", *p);

		text[i % WIDTH] = isprint(*p) ? *p : '.';

		p++;
		i++;

		if ((i % WIDTH) == 0) {
			printf(": %s\n", text);
		}
	}

	if ((i % WIDTH) != 0) {
		printf("%*s: %s\n", (WIDTH - (i % WIDTH)) * 3, " ", text);
	}

	return addr;
}
int dump_sector(uint8_t *CardID, uint8_t sector_addr) {
	int ret;
	char buffer[1024];
	uint8_t SectorKeyA[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	uint8_t SectorKeyB[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t *SectorKey;
	SectorKey = SectorKeyA;

	printf("Auth Sector (0x%02X) with key 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X ",
			sector_addr, SectorKey[0], SectorKey[1], SectorKey[2], SectorKey[3],
			SectorKey[4]);

	ret = MFRC522_Auth((uint8_t) PICC_AUTHENT1A, (uint8_t) sector_addr,
			(uint8_t*) SectorKey , (uint8_t*) CardID);
	if (ret == MI_OK) {
		printf("OK\r\n");
	} else {
		printf("Failed\r\n");
	}

	printf("Read block address 0x00 ....");
	ret = MFRC522_Read(0x00, buffer);
	ret += MFRC522_Read(0x01, buffer + ret / 8);
	ret += MFRC522_Read(0x02, buffer + ret / 8);
	ret += MFRC522_Read(0x03, buffer + ret / 8);
	if (ret <= 0) {
		printf("Failed\r\n");
	} else {
		printf("OK read %d bits\r\n", ret);
		dump(buffer, ret / 8);
	}
	return 0;
}
int main(void) {
	MFRC522_Status_t ret;
	int ret_int;
	//Recognized card ID
	uint8_t CardID[5] = { 0x00, };

	//My cards id
	//I read them with program below, and write this here
	uint8_t MyID[5] = { 0x43, 0xdc, 0x52, 0xb6, 0x7b    //My card on my keys
			};
	uint8_t sector_addr;

	MFRC522_Init('A');
	sector_addr = 1;
	while (1) {

		//If any card detected
		if (MFRC522_Check(CardID) == MI_OK) {
			printf("Card detected    0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\r\n",
					CardID[0], CardID[1], CardID[2], CardID[3], CardID[4]);
			ret_int = MFRC522_SelectTag(CardID);
			if (ret_int == 0) {
				printf("Card Select Failed\r\n");
				goto End;
			} else {
				printf("Card Selected, Type:%s\r\n", MFRC522_DumpType(MFRC522_ParseType(ret_int)));
			}
			{
				int i;
				for (i = 0x00; i < 0x4; i++) {
					dump_sector(CardID, i);
				}

			}
		}
		End: fflush(stdout);
		MFRC522_Halt();
		sleep(1);
	}
	return 0;
}
