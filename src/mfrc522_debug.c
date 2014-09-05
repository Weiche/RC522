#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "mfrc522.h"
#define WIDTH   (18)
#ifndef RELEASE
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
int __Dump_Sector(uint8_t *CardID, uint8_t sector_addr) {
	int ret;
	char buffer[1024] = "";
	uint8_t SectorKeyA[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	uint8_t SectorKeyB[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t *SectorKey;
	uint8_t Write_Data[] = "GitetsuTokyoTech";
	SectorKey = SectorKeyA;

	printf(
			"Auth Sector (0x%02X) with key 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X ...",
			sector_addr, SectorKey[0], SectorKey[1], SectorKey[2], SectorKey[3],
			SectorKey[4]);

	ret = MFRC522_Auth((uint8_t) PICC_AUTHENT1A, (uint8_t) sector_addr,
			(uint8_t*) SectorKey, (uint8_t*) CardID);
	if (ret == MI_OK) {
		printf("OK\r\n");
#ifdef TEST_WRITE
		printf("Try to write block 1 with 16 byte data...");
		ret = MFRC522_Write(0x01,Write_Data);
		if( ret == MI_OK ) {
			puts("OK");
		} else {
			printf("Failed, error 0x%02X\r\n",ret);
		}
#endif
		printf("Read block address 0x00 ....");
		ret = MFRC522_Read(sector_addr + 0x00, buffer);
		ret += MFRC522_Read(sector_addr + 0x01, buffer + ret / 8);
		ret += MFRC522_Read(sector_addr + 0x02, buffer + ret / 8);
		ret += MFRC522_Read(sector_addr + 0x03, buffer + ret / 8);
		if (ret <= 0) {
			printf("Failed\r\n");
		} else {
			printf("OK read %d bits\r\n", ret);
			dump(buffer, ret / 8);
		}
		return 0;
	} else {
		printf("Failed\r\n");
		return -1;
	}

}
int MFRC522_CardDump(uint8_t *CardID) {
	int ret_int = 0;
	printf(
			"Card detected    0x%02X 0x%02X 0x%02X 0x%02X, Check Sum = 0x%02X\r\n",
			CardID[0], CardID[1], CardID[2], CardID[3], CardID[4]);
	ret_int = MFRC522_SelectTag(CardID);
	if (ret_int == 0) {
		printf("Card Select Failed\r\n");
		return -1;
	} else {
		printf("Card Selected, Type:%s\r\n",
				MFRC522_TypeToString(MFRC522_ParseType(ret_int)));
	}
	ret_int = 0;
	{
		int i;
		for (i = 0x0; i < 0x40; i += 4) {
			ret_int |= __Dump_Sector(CardID, i);
		}
	}
	return 0;
}
const char* Reg_ToString[] = {
//Page 0: Command and Status
		"MFRC522_REG_RESERVED00",//0x00
		"MFRC522_REG_COMMAND",		//0x01
		"MFRC522_REG_COMM_IE_N",		//0x02
		"MFRC522_REG_DIV1_EN",		//0x03
		"MFRC522_REG_COMM_IRQ",		//0x04
		"MFRC522_REG_DIV_IRQ",		//0x05
		"MFRC522_REG_ERROR",		//0x06
		"MFRC522_REG_STATUS1",		//0x07
		"MFRC522_REG_STATUS2",		//0x08
		"MFRC522_REG_FIFO_DATA",		//0x09
		"MFRC522_REG_FIFO_LEVEL",		//0x0A
		"MFRC522_REG_WATER_LEVEL",		//0x0B
		"MFRC522_REG_CONTROL",		//0x0C
		"MFRC522_REG_BIT_FRAMING",		//0x0D
		"MFRC522_REG_COLL",		//0x0E
		"MFRC522_REG_RESERVED01",		//0x0F
//Page 1: Command
		"MFRC522_REG_RESERVED10",		//0x10
		"MFRC522_REG_MODE",		//0x11
		"MFRC522_REG_TX_MODE",		//0x12
		"MFRC522_REG_RX_MODE",		//0x13
		"MFRC522_REG_TX_CONTROL",		//0x14
		"MFRC522_REG_TX_AUTO",		//0x15
		"MFRC522_REG_TX_SELL",		//0x16
		"MFRC522_REG_RX_SELL",		//0x17
		"MFRC522_REG_RX_THRESHOLD",		//0x18
		"MFRC522_REG_DEMOD",		//0x19
		"MFRC522_REG_RESERVED11",		//0x1A
		"MFRC522_REG_RESERVED12",		//0x1B
		"MFRC522_REG_MIFARE",		//0x1C
		"MFRC522_REG_RESERVED13",		//0x1D
		"MFRC522_REG_RESERVED14",		//0x1E
		"MFRC522_REG_SERIALSPEED",		//0x1F
//Page 2: CFG
		"MFRC522_REG_RESERVED20",		//0x20
		"MFRC522_REG_CRC_RESULT_M",		//0x21
		"MFRC522_REG_CRC_RESULT_L",		//0x22
		"MFRC522_REG_RESERVED21",		//0x23
		"MFRC522_REG_MOD_WIDTH",		//0x24
		"MFRC522_REG_RESERVED22",		//0x25
		"MFRC522_REG_RF_CFG",		//0x26
		"MFRC522_REG_GS_N",		//0x27
		"MFRC522_REG_CWGS_PREG",		//0x28
		"MFRC522_REG__MODGS_PREG",		//0x29
		"MFRC522_REG_T_MODE",		//0x2A
		"MFRC522_REG_T_PRESCALER",		//0x2B
		"MFRC522_REG_T_RELOAD_H",		//0x2C
		"MFRC522_REG_T_RELOAD_L",		//0x2D
		"MFRC522_REG_T_COUNTER_VALUE_H",		//0x2E
		"MFRC522_REG_T_COUNTER_VALUE_L",		//0x2F
//Page 3:TestRegister
		"MFRC522_REG_RESERVED30",		//0x30
		"MFRC522_REG_TEST_SEL1",		//0x31
		"MFRC522_REG_TEST_SEL2",		//0x32
		"MFRC522_REG_TEST_PIN_EN",		//0x33
		"MFRC522_REG_TEST_PIN_VALUE",		//0x34
		"MFRC522_REG_TEST_BUS",		//0x35
		"MFRC522_REG_AUTO_TEST",		//0x36
		"MFRC522_REG_VERSION",		//0x37
		"MFRC522_REG_ANALOG_TEST",		//0x38
		"MFRC522_REG_TEST_ADC1",		//0x39
		"MFRC522_REG_TEST_ADC2",		//0x3A
		"MFRC522_REG_TEST_ADC0",		//0x3B
		"MFRC522_REG_RESERVED31",		//0x3C
		"MFRC522_REG_RESERVED32",		//0x3D
		"MFRC522_REG_RESERVED33",		//0x3E
		"MFRC522_REG_RESERVED34"		//0x3F
		};
void MFRC522_RegDump(uint8_t Reg_Addr) {
	printf("Reg:%sAddr:0x%02X,Value:0x%02X\r\n", Reg_ToString[Reg_Addr],
			Reg_Addr, MFRC522_ReadRegister(Reg_Addr));
}
#endif
