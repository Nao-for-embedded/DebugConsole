#!/usr/bin/env python3

import RPi.GPIO as GPIO # RPi.GPIOモジュールを使用
import time             # 経過時間計測用
import math

# GPIOのピン番号を割り当てる
gpio_alwaysHigh = 8 
gpio_runClkTestPin = 22
gpio_cycle1sec = 15 

# GPIO番号指定の準備
GPIO.setmode(GPIO.BCM)

# 出力ポートの設定
GPIO.setup(gpio_alwaysHigh, GPIO.OUT)
GPIO.setup(gpio_runClkTestPin, GPIO.OUT)
GPIO.setup(gpio_cycle1sec, GPIO.OUT)

# 入力ポートの設定
#GPIO.setup(gpio_sw, GPIO.IN, pull_up_down=GPIO.PUD_UP)
#
## スイッチの状態を取得
#sw = GPIO.input(gpio_sw)


#######################
# メインルーチン       #
#######################
GPIO.output(gpio_alwaysHigh, 1)
GPIO.output(gpio_cycle1sec, 1)
print("cycle high")

toggle_flag = True
time_sta = time.time()
preSpendTime = 0
while 1:
    # 10秒経過したら
    spendtime_ms = (time.time() - time_sta) * 1000
    if( spendtime_ms > 10000 ):
        break
    
    if( toggle_flag ) :
        toggle_flag = False
        GPIO.output(gpio_runClkTestPin, 1)
    else :
        toggle_flag = True
        GPIO.output(gpio_runClkTestPin, 0)

    spendtime = math.floor(spendtime_ms / 1000 )
    if( spendtime != preSpendTime ):
        preSpendTime = spendtime
        if( ( spendtime % 2) == 0 ):
            GPIO.output(gpio_cycle1sec, 1)
            print("cycle high")
        else :
            GPIO.output(gpio_cycle1sec, 0)
            print("cycle low")

# 後処理 GPIOを解放
GPIO.cleanup(gpio_alwaysHigh)
GPIO.cleanup(gpio_runClkTestPin)
GPIO.cleanup(gpio_cycle1sec)
