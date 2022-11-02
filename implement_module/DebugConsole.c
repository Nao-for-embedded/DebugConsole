#include "DebugConsole.h"
#include "DebugConsole_hardware.h"

void DebugConsole_Write( const char* message )
{
	char c;

	for(;;)
	{
		c = *message++;
		if( c == 0x00 ) break;

		// スタートビット
		DEBUGCONSOLE_DATA_LOW();
		DEBUGCONSOLE_CLK_HIGH(); DEBUGCONSOLE_WAIT();
		DEBUGCONSOLE_DATA_HIGH();
		DEBUGCONSOLE_CLK_LOW(); DEBUGCONSOLE_WAIT();

		// データビット bit6
		if( c & 0x40 )
		{
			DEBUGCONSOLE_DATA_HIGH();
		}
		else
		{
			DEBUGCONSOLE_DATA_LOW();
		}
		DEBUGCONSOLE_CLK_HIGH(); DEBUGCONSOLE_WAIT();
		DEBUGCONSOLE_CLK_LOW(); DEBUGCONSOLE_WAIT();
		
		// データビット bit5
		if( c & 0x20 )
		{
			DEBUGCONSOLE_DATA_HIGH();
		}
		else
		{
			DEBUGCONSOLE_DATA_LOW();
		}
		DEBUGCONSOLE_CLK_HIGH(); DEBUGCONSOLE_WAIT();
		DEBUGCONSOLE_CLK_LOW(); DEBUGCONSOLE_WAIT();
		
		// データビット bit4
		if( c & 0x10 )
		{
			DEBUGCONSOLE_DATA_HIGH();
		}
		else
		{
			DEBUGCONSOLE_DATA_LOW();
		}
		DEBUGCONSOLE_CLK_HIGH(); DEBUGCONSOLE_WAIT();
		DEBUGCONSOLE_CLK_LOW(); DEBUGCONSOLE_WAIT();
		
		// データビット bit3
		if( c & 0x08 )
		{
			DEBUGCONSOLE_DATA_HIGH();
		}
		else
		{
			DEBUGCONSOLE_DATA_LOW();
		}
		DEBUGCONSOLE_CLK_HIGH(); DEBUGCONSOLE_WAIT();
		DEBUGCONSOLE_CLK_LOW(); DEBUGCONSOLE_WAIT();
		
		// データビット bit2
		if( c & 0x04 )
		{
			DEBUGCONSOLE_DATA_HIGH();
		}
		else
		{
			DEBUGCONSOLE_DATA_LOW();
		}
		DEBUGCONSOLE_CLK_HIGH(); DEBUGCONSOLE_WAIT();
		DEBUGCONSOLE_CLK_LOW(); DEBUGCONSOLE_WAIT();

		// データビット bit1
		if( c & 0x02 )
		{
			DEBUGCONSOLE_DATA_HIGH();
		}
		else
		{
			DEBUGCONSOLE_DATA_LOW();
		}
		DEBUGCONSOLE_CLK_HIGH(); DEBUGCONSOLE_WAIT();
		DEBUGCONSOLE_CLK_LOW(); DEBUGCONSOLE_WAIT();

		// データビット bit0
		if( c & 0x01 )
		{
			DEBUGCONSOLE_DATA_HIGH();
		}
		else
		{
			DEBUGCONSOLE_DATA_LOW();
		}
		DEBUGCONSOLE_CLK_HIGH(); DEBUGCONSOLE_WAIT();
		DEBUGCONSOLE_CLK_LOW(); DEBUGCONSOLE_WAIT();

		// ストップビット
		DEBUGCONSOLE_DATA_HIGH();
		DEBUGCONSOLE_CLK_HIGH(); DEBUGCONSOLE_WAIT();
		DEBUGCONSOLE_DATA_LOW();
		DEBUGCONSOLE_CLK_LOW(); DEBUGCONSOLE_WAIT();
	}
}
