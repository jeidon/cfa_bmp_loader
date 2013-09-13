#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <cfaf128128b-0145t.h>
#include <unistd.h>

#pragma pack(push, 1)
typedef struct tagBITMAPFILEHEADER
{
	short bfType;					//specifies the file type
	int bfSize;					//specifies the size in bytes of the bitmap file
    short bfReserved1;			//reserved; must be 0
    short bfReserved2;			//reserved; must be 0
    int bOffBits;				//specifies the offset in bytes from the bitmapfileheader to the bitmap bits
}BITMAPFILEHEADER;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct tagBITMAPINFOHEADER
{
    int biSize;			//specifies the number of bytes required by the struct
    int biWidth;			//specifies width in pixels
    int biHeight;			//species height in pixels
    short biPlanes;			//specifies the number of color planes, must be 1
    short biBitCount;		//specifies the number of bit per pixel
    int biCompression;	//specifies the type of compression
    int biSizeImage;  	//size of image in bytes
    int biXPelsPerMeter;	//number of pixels per meter in x axis
    int biYPelsPerMeter;	//number of pixels per meter in y axis
    int biClrUsed;  		//number of colors used by the bitmap
    int biClrImportant;	//number of colors that are important
}BITMAPINFOHEADER;
#pragma pack(pop)

unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
    FILE *filePtr; //our file pointer
    BITMAPFILEHEADER bitmapFileHeader; //our bitmap file header
    unsigned char *bitmapImage;  //store image data
    int imageIdx=0;  //image index counter
    
    //open filename in read binary mode
    filePtr = fopen(filename,"rb");
    if (filePtr == NULL) {
		printf("Couldn't open file\n");
        return NULL;
	}
	
    //read the bitmap file header
    fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER),1,filePtr);

    //verify that this is a bmp file by check bitmap id
    if (bitmapFileHeader.bfType !=0x4D42)
    {
		printf("Expected 0x4d42, found: %x\n", bitmapFileHeader.bfType);
        printf("File is not a bmp\n");
		fclose(filePtr);
        return NULL;
    }

    //read the bitmap info header
    fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER),1,filePtr);

    //move file point to the begging of bitmap data
    fseek(filePtr, bitmapFileHeader.bOffBits, SEEK_SET);

    //allocate enough memory for the bitmap image data
    bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

    //verify memory allocation
    if (!bitmapImage)
    {
		printf("Error 2\n");
        free(bitmapImage);
        fclose(filePtr);
        return NULL;
    }
	
    //read in the bitmap image data
    fread(bitmapImage,bitmapInfoHeader->biSizeImage,1,filePtr);
	
    //make sure bitmap image data was read
    if (bitmapImage == NULL)
    {
		printf("Error 3\n");
        fclose(filePtr);
        return NULL;
    }
	
	for(imageIdx=0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx+=3){
		tft_writeColor(bitmapImage[imageIdx+2], bitmapImage[imageIdx+1], bitmapImage[imageIdx]);
	}
    //close file and return bitmap iamge data
    fclose(filePtr);
    return bitmapImage;
}

static void pabort(const char *s){
	perror(s);
	abort();
}

int main(int argc, char *argv[])
{
	BITMAPINFOHEADER bitmapInfoHeader;
	//unsigned char *bitmapData;
	int ret=0;
	
	ret = init_tft(1);
	if(ret==-1)
	{
		printf("init error: %i\n", errno);
		pabort("Unable to initialize the display");
	}
	
	while(1)
	{
		setOrientation(0);
		
		// Write to GRAM
		SPI3W_sendCommand(0x2C);
		for(int i=0; i<128; i++) {
			for(int j=0; j<128; j++) {
				tft_writeColor(255, 0, 0);
			}
		}
	
		for(int x=0;x<4;x++)
		{
			setOrientation(x);
			SPI3W_sendCommand(0x2C);
			LoadBitmapFile("CFAF128128B-0145T.bmp",&bitmapInfoHeader);
		}
	}
	return ret;
}

