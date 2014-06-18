#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include "elisa3-lib.h"

#define BUFF_SIZE 1024

int main(int argc, char *argv[]) {

    int robotAddress[1];
    int redLed, greenLed, blueLed;
    int lspeed, rspeed;
    unsigned int smallLeds;
    unsigned int flagsTx[2];
    char buff[BUFF_SIZE]={0};
    unsigned int matched = 0;
    FILE *cmdFile;

    // init the communication with the RF module, even if the address is 0 tell the library we will control 1 robot
    startCommunication(robotAddress, 1);

    // text file parameters are:
    // ADDRESS LSPEED RSPEED RED GREEN BLUE GREENLEDS
    // - LSPEED/RSPEED: -128..127 (default speed instead of 0)
    // - RED/GREEN/BLUE: 0..100
    // - GREENLEDS: 0..0xFF (each bits correspond to a led)
    // - FLAGS(1)
    // - FLAGS(2)
    if ((cmdFile = fopen("commands.csv", "r")) == NULL) {
        printf("Command file commands.csv doesn't exist!\r\n");
        return 1;
    }

    fgets(buff, sizeof buff, cmdFile);	// skip first line (table header)
    memset(buff, 0, BUFF_SIZE);

    while (fgets(buff, sizeof buff, cmdFile) != NULL ) {

        matched = sscanf(buff, "%d;%d;%d;%d;%d;%d;%d;%d;%d", &robotAddress[0], &lspeed, &rspeed, &redLed, &greenLed, &blueLed, &smallLeds, &flagsTx[0], &flagsTx[1]);
        printf("%d;%d;%d;%d;%d;%d;%d;%d;%d\n", robotAddress[0], lspeed, rspeed, redLed, greenLed, blueLed, smallLeds, flagsTx[0], flagsTx[1]);

        if(sendMessageToRobot(robotAddress[0], redLed, greenLed, blueLed, (char*)flagsTx, lspeed, rspeed, smallLeds, 100000) == 1) {
            printf("cannot send msg to robot %d\r\n", robotAddress[0]);
        }

        memset(buff, 0, BUFF_SIZE);

    }

    fclose(cmdFile);

    stopCommunication();

	return 0;

}

