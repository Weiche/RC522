/*
 * main.c
 *
 *  Created on: 2014��9��2��
 *      Author: Administrator
 */

#include <stdio.h>
#include <stdint.h>
#include "mfrc522.h"
int main(void){
    //Recognized card ID
    uint8_t CardID[5];
    //My cards id
    //I read them with program below, and write this here
    uint8_t MyID[5] = {
        0x43, 0xdc, 0x52, 0xb6, 0x7b    //My card on my keys
    };
    char buffer[50];
    MFRC522_Init();
    while (1) {
        //If any card detected
        if (MFRC522_Check(CardID) == MI_OK) {
            puts("Card detected    ");
            printf("0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x", CardID[0], CardID[1], CardID[2], CardID[3], CardID[4]);
        }
    }
	return 0;
}
