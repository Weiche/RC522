/**
 * Read a card using a mfrc522 reader on your SPI interface
 * Pin layout should be as follows (on Arduino Uno):
 * MOSI: Pin 11 / ICSP-4
 * MISO: Pin 12 / ICSP-1
 * SCK: Pin 13 / ISCP-3
 * SS: Pin 10
 * RST: Pin 9
 *
 * Script is based on the script of Miguel Balboa.
 * New cardnumber is printed when card has changed. Only a dot is printed
 * if card is the same.
 *
 * @version 0.1
 * @author Henri de Jong
 * @since 06-01-2013
 */

#include "RFID.h"
#include <stdio.h>
extern RFID_Typedef RFID;
#define SS_PIN 10
#define RST_PIN 9

// Setup variables:
int serNum0;
int serNum1;
int serNum2;
int serNum3;
int serNum4;

void setup() {
	RFID_init();
}

void loop() {

	if (RFID_isCard()) {
		printf("Card found, Try to read serial\r\n");
		if (RFID_readCardSerial()) {
			if (RFID.serNum[0] != serNum0 && RFID.serNum[1] != serNum1
					&& RFID.serNum[2] != serNum2 && RFID.serNum[3] != serNum3
					&& RFID.serNum[4] != serNum4) {
				/* With a new cardnumber, show it. */
				printf("Card found");
				serNum0 = RFID.serNum[0];
				serNum1 = RFID.serNum[1];
				serNum2 = RFID.serNum[2];
				serNum3 = RFID.serNum[3];
				serNum4 = RFID.serNum[4];

				printf("Cardnumber:0x%X 0x%X 0x%X 0x%X 0x%X \r\n",RFID.serNum[0],RFID.serNum[1],RFID.serNum[2],RFID.serNum[3],RFID.serNum[4]);
			} else {
				/* If we have the same ID, just write a dot. */
				puts(".");
			}
		}
	}

	RFID_halt();
}

int main(void){
	printf("RC522 Test Start\r\n");
	setup();
	while(1){
		loop();
	}
}
