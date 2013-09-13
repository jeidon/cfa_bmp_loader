#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <inttypes.h>
#include "spi-driver.h"
#include "cfaf128128b-0145t.h"

#define pabort(s) {perror(s); abort();}

typedef struct _6bit { int val:6;} uint6_t;

typedef struct _bitDStruct {
	unsigned int b0:1;
	unsigned int b1:1;
	unsigned int b2:1;
	unsigned int b3:1;
	unsigned int b4:1;
	unsigned int b5:1;
	unsigned int b6:1;
	unsigned int b7:1;
} bitDStruct;

typedef struct _Transfer {
	uint8_t data;
	uint8_t type:1;
	uint8_t pad:7;
} Transfer;

static int initResetPin(){
	int ret = 0;
	FILE* ex = fopen("/sys/class/gpio/export","w");
	fprintf(ex,"21");
	fclose(ex);
	
	if(errno!=0)
		errno=0;
		
	FILE* pindir = fopen("/sys/class/gpio/gpio21/direction","w");
	fprintf(pindir,"out");
	fclose(pindir);
	
	FILE* pinval = fopen("/sys/class/gpio/gpio21/value","w");
	fprintf(pinval,"0");
	fclose(pinval);
	
	return ret;
}

static void resetClear(){
	FILE* pin = fopen("/sys/class/gpio/gpio21/value","w");
	fprintf(pin,"0");
	fclose(pin);
}

static void resetSet(){
	FILE* pin = fopen("/sys/class/gpio/gpio21/value","w");
	fprintf(pin,"1");
	fclose(pin);
}

int SPI3W_sendCommand(char cmd){
	int ret=0;

	Transfer f = {
		.data = cmd,
		.type = 0,
	};
	
	void* structure = &f;
	
	//printf("%i\n", sizeof(Transfer));
	ret = SPIWriteWord(structure);
	
	if(ret == -1)
		pabort("Invalid Command");
		
	return ret;
}

int SPI3W_sendData(char datal){
	int ret=0;

	Transfer f = {
		.data = datal,
		.type = 1,
	};

	void* structure = &f;
	
	//printf("%i\n", sizeof(Transfer));
	ret = SPIWriteWord(structure);
	
	if(ret == -1)
		pabort("Invalid Data");
		
	return ret;
}

int init_tft(int deviceNum) {
	int ret=0;
	int SPISpeed = 2000000;
	ret = initResetPin();
	
	if(ret == -1){
		pabort("Unable to initialize reset PIN");
	}
	
	ret = SPIInit(deviceNum, 9, SPISpeed);
	if(ret < 0){
		pabort("Unable to initialize SPI");
	}
		
	//***************************RESET LCD Driver*******************************
	// SET RST Pin high
	resetSet();
	usleep(1000);

	// SET RST Pin low
	resetClear();
	usleep(20000);

	// SET RST Pin high
	resetSet();
	usleep(120000);

	SPI3W_sendCommand(0x11); // Sleep out and Charge Pump on
	usleep(120000);

	SPI3W_sendCommand(0xB1); //SETPWCTR
	SPI3W_sendData(0x02);
	SPI3W_sendData(0x35);
	SPI3W_sendData(0x36);
	
	SPI3W_sendCommand(0xB2); //SETDISPLAY
	SPI3W_sendData(0x02);
	SPI3W_sendData(0x35);
	SPI3W_sendData(0x36);
	
	SPI3W_sendCommand(0xB3); //Doesn't exist
	SPI3W_sendData(0x02);
	SPI3W_sendData(0x35);
	SPI3W_sendData(0x36);
	SPI3W_sendData(0x02);
	SPI3W_sendData(0x35);
	SPI3W_sendData(0x36);

	SPI3W_sendCommand(0xB4); //SETCYC
	SPI3W_sendData(0x07);
	
	SPI3W_sendCommand(0xC0); //SETSTBA
	SPI3W_sendData(0xa2);
	SPI3W_sendData(0x02);
	SPI3W_sendData(0x04);
	
	SPI3W_sendCommand(0xc1); //Doesn't exist
	SPI3W_sendData(0xc5);

	SPI3W_sendCommand(0xc2); //Doesn't exist
	SPI3W_sendData(0x0d);
	SPI3W_sendData(0x00);
	
	SPI3W_sendCommand(0xc3); // SETID
	SPI3W_sendData(0x8d);
	SPI3W_sendData(0x1a);
	
	SPI3W_sendCommand(0xc4); //Doesn't exist
	SPI3W_sendData(0x8d);
	SPI3W_sendData(0xee);
	
	SPI3W_sendCommand(0xc5); //Doesn't exist
	SPI3W_sendData(0x09);
	
	SPI3W_sendCommand(0xE0); //Gamma setting For LGIT Panel
	SPI3W_sendData(0x0a);
	SPI3W_sendData(0x1c);
	SPI3W_sendData(0x0c);
	SPI3W_sendData(0x14);
	SPI3W_sendData(0x33);
	SPI3W_sendData(0x2b);
	SPI3W_sendData(0x24);
	SPI3W_sendData(0x28);
	SPI3W_sendData(0x27);
	SPI3W_sendData(0x25);
	SPI3W_sendData(0x2C);
	SPI3W_sendData(0x39);
	SPI3W_sendData(0x00);
	SPI3W_sendData(0x05);
	SPI3W_sendData(0x03);
	SPI3W_sendData(0x0d);
	
	SPI3W_sendCommand(0xE1); //Doesn't exist
	SPI3W_sendData(0x0a);
	SPI3W_sendData(0x1c);
	SPI3W_sendData(0x0c);
	SPI3W_sendData(0x14);
	SPI3W_sendData(0x33);
	SPI3W_sendData(0x2b);
	SPI3W_sendData(0x24);
	SPI3W_sendData(0x28);
	SPI3W_sendData(0x27);
	SPI3W_sendData(0x25);
	SPI3W_sendData(0x2D);
	SPI3W_sendData(0x3a);
	SPI3W_sendData(0x00);
	SPI3W_sendData(0x05);
	SPI3W_sendData(0x03);
	SPI3W_sendData(0x0d);
	
	SPI3W_sendCommand(0x3A); //Doesn't exist
	SPI3W_sendData(0x06);

	SPI3W_sendCommand(0x29); //Display On
	usleep(150);
}
	
void tft_writeColor(char r,char g,char b){
	SPI3W_sendData(b);
	SPI3W_sendData(g);
	SPI3W_sendData(r);
}

void setOrientation(int orientation) {
	switch(orientation) {
		case 0:
			SPI3W_sendCommand(0x2A); //Column address set
			SPI3W_sendData(0x00);
			SPI3W_sendData(0x02);
			SPI3W_sendData(0x00);
			SPI3W_sendData(0x81);
			
			SPI3W_sendCommand(0x2B); //Row address set
			SPI3W_sendData(0x00);
			SPI3W_sendData(0x01);
			SPI3W_sendData(0x00);
			SPI3W_sendData(0x80);

			SPI3W_sendCommand(0x36); //Change pixel RGB order
			SPI3W_sendData(0x40);
			break;
		case 1: 
			SPI3W_sendCommand(0x2A); //Column address set
			SPI3W_sendData(0x00);
			SPI3W_sendData(0x03);
			SPI3W_sendData(0x00);
			SPI3W_sendData(0x82);
			
			SPI3W_sendCommand(0x2B); //Row address set
			SPI3W_sendData(0x00);
			SPI3W_sendData(0x02);
			SPI3W_sendData(0x00);
			SPI3W_sendData(0x81);

			SPI3W_sendCommand(0x36); //Change pixel RGB order
			SPI3W_sendData(0xE0);
			break;
		case 2: 
			SPI3W_sendCommand(0x2A); //Column address set
			SPI3W_sendData(0x00);
			SPI3W_sendData(0x02);
			SPI3W_sendData(0x00);
			SPI3W_sendData(0x81);
			
			SPI3W_sendCommand(0x2B); //Row address set
			SPI3W_sendData(0x00);
			SPI3W_sendData(0x03);
			SPI3W_sendData(0x00);
			SPI3W_sendData(0x82);

			SPI3W_sendCommand(0x36); //Change pixel RGB order
			SPI3W_sendData(0x80);
			break;
		case 3: 
			SPI3W_sendCommand(0x2A); //Column address set
			SPI3W_sendData(0x00);
			SPI3W_sendData(0x01);
			SPI3W_sendData(0x00);
			SPI3W_sendData(0x80);
			
			SPI3W_sendCommand(0x2B); //Row address set
			SPI3W_sendData(0x00);
			SPI3W_sendData(0x02);
			SPI3W_sendData(0x00);
			SPI3W_sendData(0x81);

			SPI3W_sendCommand(0x36); //Change pixel RGB order
			SPI3W_sendData(0x20);
			break;
	}
}
