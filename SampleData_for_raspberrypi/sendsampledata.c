#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "module/DebugConsole.h"
#include "module/DebugConsole_hardware.h"


#define PULLDOWN 1
#define PULLUP 2

#define STRN 16

int main(int argc, char* argv[] )
{
	FILE* fp;
	char str[STRN];

	wiringPiSetupGpio () ;

	pinMode (gpio_outClk, OUTPUT) ;
	pinMode (gpio_outData, OUTPUT) ;
	pullUpDnControl( gpio_outClk, PULLUP);
	pullUpDnControl( gpio_outData, PULLUP);

	fp = fopen( "sampledata.txt", "r" );
	if( fp == NULL )
	{
		printf("sampledata.txt is not found.\n");
		return -1;
	}

	for(;;)
	{
		if( fgets(str, STRN, fp) == NULL ) break;

		DebugConsole_Write(str);
		printf("%s", str);
	}


	fclose(fp);

	return 0;
}
