#include <M5StickCPlus.h>
#include "BluetoothSerial.h"
#include <freertos/FreeRTOS.h>

enum eBit{ eBit_Low, eBit_High, eBit_Start, eBit_Stop };
enum eMode{ eMode_Initialize, eMode_Monitor, eMode_Analyze };

#define NUMOF(x)  ( (x) / (x[0]) )
#define  CLKPIN   32    /* クロックのポート番号 */
#define  DATPIN   33    /* データのポート番号 */

#define  CLKPIN_FB   35    /* クロックのポート番号 */
#define  DATPIN_FB   36    /* データのポート番号 */

#define LINE 26
#define CHAR_LINE (20+1)         /* 一行に入る文字数 + 1 */
TaskHandle_t taskHandle[2];
QueueHandle_t Queue_Bit;
QueueHandle_t Queue_Char;
BluetoothSerial SerialBT;
int g_line = 0;

#define STRN 1024
char g_str[STRN+1];

/* 信号解析 */
void  TkMain( void *param )
{
  Serial.println("TkMain run!");

  int strLength = 0;
  memset( g_str, '\0', sizeof(g_str));
  int line = 1;
  Display_Update( eMode_Monitor );
  while( 1 )
  {
    vTaskDelay(100);
    char c;
    while( xQueueReceive( Queue_Char, &c, 0 ) == pdTRUE )
    {
      Serial.print(c);
      if( c == '\n' )
      {
        SerialBT.print(g_str);
        SerialBT.print("\r\n");
      }
      if( strLength <= STRN )
      {
        g_str[strLength++] = c;
        Display_PrintLine(line, g_str, WHITE, BLACK);
      }
      if( c == '\n' )
      {
        strLength = 0;
        memset( g_str, '\0', sizeof(g_str));
        line = line + 1;
        if( line >= LINE )
        {
          line = 1;
        }
      }
    }

    Display_Update( eMode_Monitor );
  }
}



enum eMode g_CurrentMode = eMode_Initialize;
int g_DisplayCount = 0;
char g_Message[LINE][CHAR_LINE];
void Display_Initialize( void )
{
  M5.Lcd.setTextSize(2);
}

// 描画
void Display_Update( enum eMode mode )
{
  if( mode != g_CurrentMode )
  {
      M5.Lcd.setCursor(0,0);
      M5.Lcd.setTextSize(1);
      M5.Lcd.setTextWrap(false);
      M5.Lcd.fillScreen(BLACK);

      g_CurrentMode = mode;
  }

  // 1秒に一回更新する
  if( (g_DisplayCount % 1000) == 0 )
  {
    Display_Header();

    M5.update();
    if( M5.BtnB.wasPressed())
    {
        M5.Lcd.fillScreen(BLACK);
        SerialBT.println("Bluetooth is active.");
    }
  }
  Display_Body();

  g_DisplayCount += 100;
}

void Display_PrintLine( int line, const char* msg, uint16_t textColor, uint16_t bgColor)
{
  M5.Lcd.setCursor(0, line*8);
  M5.Lcd.fillRect( 0, line*8, M5.Lcd.width(), 8, bgColor );
  M5.Lcd.setTextColor(textColor);
  M5.Lcd.print(msg);
//  Serial.println(msg);
}

void Display_Header(void)
{
  float vbat = M5.Axp.GetBatVoltage(); // 3.0~4.2V
  float ibat = M5.Axp.GetBatCurrent();
  float batteryRate = (vbat - 3.0 ) / 1.2 * 100; 
  char msg[64];

  sprintf(msg, " Bat %.0f%% %.1fV %.1fmA\n", batteryRate, vbat, ibat);

  if( batteryRate > 80.0 )
  {
    Display_PrintLine(0, msg, GREEN, BLACK);
  }
  else if( batteryRate > 40.0 )
  {
    Display_PrintLine(0, msg, WHITE, BLACK);
  }
  else
  {
    Display_PrintLine(0, msg, RED, BLACK);
  }
}

// ボディ表示
void Display_Body(void)
{
  int i;
  for( i=0 ; i<LINE ; i++)
  {
//    Display_PrintLine(i+1, g_Message[i], WHITE, BLACK);
  }
}

void DisplayMessage( const char* msg)
{
  char tmp[CHAR_LINE+1];
  strncpy( tmp, msg, CHAR_LINE );
  tmp[CHAR_LINE] = '\0';

  g_line = g_line + 1;
  Display_PrintLine(g_line, tmp, WHITE, BLACK);
  if( g_line >= LINE )
  {
    g_line = 0;
  }
}


// フッター表示（なし）

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// スキャン
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum eSignal
{
  eClockHigh_DataHigh,
  eClockHigh_DataLow,
  eClockLow,
};

void IRAM_ATTR onClockChange(void);

void Scan_Initialize(void)
{
  M5.Lcd.print("ScanInitialize\n");
  pinMode(CLKPIN, INPUT);
  pinMode(DATPIN, INPUT);

  pinMode(CLKPIN_FB, OUTPUT);
  pinMode(DATPIN_FB, OUTPUT);

  attachInterrupt(CLKPIN, onClockChange, CHANGE);
}


eSignal g_preSignal = eClockLow;

void IRAM_ATTR onClockChange(void)
{
  const char msg[] = "onClockChange";
  uint8_t clk = digitalRead(CLKPIN);
  uint8_t data = digitalRead(DATPIN);
  digitalWrite(CLKPIN_FB, clk);
  digitalWrite(DATPIN_FB, data);

  eSignal signal;
  if(clk != 0)
  {
      if( data != 0 )
      {
        signal = eClockHigh_DataHigh;        
      }
      else
      {
        signal = eClockHigh_DataLow;
      }
      switch( g_preSignal )
      {
        case eClockLow:
          g_preSignal = signal;
          break;
      }
  }
  else
  {
      eBit bit;
      switch( g_preSignal )
      {
        case eClockHigh_DataHigh:
          if( data != 0 )
          {
            bit = eBit_High;
          }
          else
          {
            bit = eBit_Stop;
          }
          xQueueSendFromISR( Queue_Bit, &bit, 0);
          break;
        case eClockHigh_DataLow:
          if( data != 0 )
          {
            bit = eBit_Start;
          }
          else
          {
            bit = eBit_Low;
          }
          xQueueSendFromISR( Queue_Bit, &bit, 0);
          break;
      }
      g_preSignal = eClockLow;
  }


//  DisplayMessage(msg);
}

/* 信号スキャン */
void  TkScan( void *param )
{
  Serial.println("TkScan run!");
  while( 1 ) {
    eBit bit;
    char c = 0;

    // スタートビット待ち
    xQueueReceive( Queue_Bit, &bit, portMAX_DELAY );
    if( bit != eBit_Start ) continue;
//    Serial.println("eBit_Start");

    c = random('A', 'Z'+1);
    if( c == 'Z' ) c = '\n';
    // キャラクタ確定
    xQueueSend( Queue_Char, &c, 0 );

    // bit6待ち
    xQueueReceive( Queue_Bit, &bit, portMAX_DELAY );
    if( bit == eBit_High )
    {
        c = ( c<<1) + 1;
    }
    else if( bit == eBit_Low )
    {
        c = ( c<<1) + 0;
    }
    else
    {
      continue;
    }

    // bit5待ち
    xQueueReceive( Queue_Bit, &bit, portMAX_DELAY );
    if( bit == eBit_High )
    {
        c = ( c<<1) + 1;
    }
    else if( bit == eBit_Low )
    {
        c = ( c<<1) + 0;
    }
    else
    {
      continue;
    }
    // bit4待ち
    xQueueReceive( Queue_Bit, &bit, portMAX_DELAY );
    if( bit == eBit_High )
    {
        c = ( c<<1) + 1;
    }
    else if( bit == eBit_Low )
    {
        c = ( c<<1) + 0;
    }
    else
    {
      continue;
    }
    // bit3待ち
    xQueueReceive( Queue_Bit, &bit, portMAX_DELAY );
    if( bit == eBit_High )
    {
        c = ( c<<1) + 1;
    }
    else if( bit == eBit_Low )
    {
        c = ( c<<1) + 0;
    }
    else
    {
      continue;
    }

    // bit2待ち
    xQueueReceive( Queue_Bit, &bit, portMAX_DELAY );
    if( bit == eBit_High )
    {
        c = ( c<<1) + 1;
    }
    else if( bit == eBit_Low )
    {
        c = ( c<<1) + 0;
    }
    else
    {
      continue;
    }

    // bit1待ち
    xQueueReceive( Queue_Bit, &bit, portMAX_DELAY );
    if( bit == eBit_High )
    {
        c = ( c<<1) + 1;
    }
    else if( bit == eBit_Low )
    {
        c = ( c<<1) + 0;
    }
    else
    {
      continue;
    }

    // bit0待ち
    xQueueReceive( Queue_Bit, &bit, portMAX_DELAY );
    if( bit == eBit_High )
    {
        c = ( c<<1) + 1;
    }
    else if( bit == eBit_Low )
    {
        c = ( c<<1) + 0;
    }
    else
    {
      continue;
    }

    // ストップビット待ち
    xQueueReceive( Queue_Bit, &bit, portMAX_DELAY );
    if( bit != eBit_Stop ) continue;
//    Serial.println("eBit_Stop");

    // キャラクタ確定
    xQueueSend( Queue_Char, &c, 0 );
  }
}

