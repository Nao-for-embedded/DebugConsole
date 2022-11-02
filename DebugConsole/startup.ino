
char bluetoothName[256];
void setup() {
  // put your setup code here, to run once:
  M5.begin();
//  M5.Lcd.setRotation(3);
//  M5.Power.begin();
//  if(!M5.Power.canControl()) {
//    //can't control.
//    M5.Lcd.print("M5.Power.canControl NG");
//  }

  Display_Initialize();
  M5.Lcd.print("serial.begin\n");
  Serial.begin(115200);

  sprintf( bluetoothName, "M5StickCPluse_%04X", (uint16_t)(ESP.getEfuseMac()>>32));
  SerialBT.begin(bluetoothName);

  Scan_Initialize();

  M5.Lcd.print("create queue\n");
  Queue_Bit = xQueueCreate(32, sizeof(eBit));
  Queue_Char = xQueueCreate(256, sizeof(char));

  M5.update();

  if( !M5.BtnA.isPressed() )
  {
//    M5.Beep.beep();
  }

  M5.Lcd.print("create TkScan\n");
  xTaskCreatePinnedToCore( TkScan, "TkScan", 0x800, NULL, 10, &taskHandle[0], 0 );
  M5.Lcd.print("create TkMain\n");
  xTaskCreatePinnedToCore( TkMain, "TkMain", 0x800, NULL, 9, &taskHandle[1], 0 );
 

//  delay(1000);
//
//  M5.Lcd.setTextSize(1);    // 変化がわかればいいので最小値
//  vTaskStartScheduler();
}

void loop() {
}