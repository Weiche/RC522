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
#include <getopt.h>
#include "mfrc522.h"
#define DISP_COMMANDLINE()	printf("RC522>")
int main(int argc, char **argv) {
	MFRC522_Status_t ret;
	int ret_int;
	//Recognized card ID
	uint8_t CardID[5] = { 0x00, };
	uint8_t sector_addr;
	static char command_buffer[1024];
	static int command_argc = 0;
	static char *command_argv[256];
	char *tp;

	ret = MFRC522_Init('A');
	if (ret < 0) {
		perror("Failed to init");
		exit(-1);
	}

	printf("User Space RC522 Application\r\n");

	while (1) {
		DISP_COMMANDLINE();
		fflush(stdin);
		scanf("%s", command_buffer);
		if (strcmp(command_buffer, "scan") == 0) {
			puts("Scanning");
			while (1) {
				ret = MFRC522_Check(CardID);
				if (ret == MI_OK) {
					MFRC522_CardDump(CardID);
					break;
				} else {
					printf(".");
				}
				fflush(stdout);
				MFRC522_Halt();
				usleep(500 * 1000);
			}

		} else if (strcmp(command_buffer, "quit") == 0 || strcmp(command_buffer, "exit") == 0 ) {
			return 0;
		} else {
			puts("Unknow command");
			puts("scan:scan card and dump");
			puts("quit:exit programe");
		}

	}
}
