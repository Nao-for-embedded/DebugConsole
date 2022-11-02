#include <unistd.h>
#include <wiringPi.h>

#define gpio_outClk 2
#define gpio_outData 17


// クロックラインをHIGHにする
#define DEBUGCONSOLE_CLK_HIGH() digitalWrite( gpio_outClk, 1 )
// クロックラインをLOWにする
#define DEBUGCONSOLE_CLK_LOW() digitalWrite( gpio_outClk, 0 )

// データラインをHIGHにする
#define DEBUGCONSOLE_DATA_HIGH() digitalWrite( gpio_outData, 1 )
// データラインをLOWにする
#define DEBUGCONSOLE_DATA_LOW() digitalWrite( gpio_outData, 0 )

// 1usウェイト
#define DEBUGCONSOLE_WAIT()	usleep(1);

