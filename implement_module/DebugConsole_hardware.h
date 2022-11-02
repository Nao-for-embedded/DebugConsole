#include <unistd.h>
#include <wiringPi.h>

#define gpio_outClk 2
#define gpio_outData 17


// �N���b�N���C����HIGH�ɂ���
#define DEBUGCONSOLE_CLK_HIGH() digitalWrite( gpio_outClk, 1 )
// �N���b�N���C����LOW�ɂ���
#define DEBUGCONSOLE_CLK_LOW() digitalWrite( gpio_outClk, 0 )

// �f�[�^���C����HIGH�ɂ���
#define DEBUGCONSOLE_DATA_HIGH() digitalWrite( gpio_outData, 1 )
// �f�[�^���C����LOW�ɂ���
#define DEBUGCONSOLE_DATA_LOW() digitalWrite( gpio_outData, 0 )

// 1us�E�F�C�g
#define DEBUGCONSOLE_WAIT()	usleep(1);

