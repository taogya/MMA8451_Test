
#define TIMER0_INTERVAL_MS 10

#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#include <RPi_Pico_TimerInterrupt.h>

Adafruit_MMA8451 mma = Adafruit_MMA8451();
RPI_PICO_Timer ITimer0(0);
bool canReadAcceleration = false;

// タイマー0割り込みハンドラー
bool Timer0Handler(struct repeating_timer *t)
{ 
  (void) t;
  // 加速度センサー読み込み許可
  canReadAcceleration = true;
  
  return true;
}

// interval_ms * 2 秒間隔で count * interval_ms 秒間点滅
void blink(uint interval_ms, uint count)
{
  bool toggle = false;
  while(count-- > 0)
  {
    delay(interval_ms);
    digitalWrite(LED_BUILTIN, toggle);
    toggle = !toggle;
  }
    digitalWrite(LED_BUILTIN, false);
}

// LEDの初期化
void SetupPin()
{
  // Builtin LED
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("Builtin LED Initialized.");
}

// シリアル初期化
void SetupSerial()
{
  // Serial初期化
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Serial Initialized.");
}

// 加速度センサー初期化
void SetupAcceleration()
{
  // 初期化
  while (!mma.begin());
  // データレート設定
  while (mma.getDataRate() != MMA8451_DATARATE_800_HZ)
    mma.setDataRate(MMA8451_DATARATE_800_HZ);
  // レンジ設定
  while (mma.getRange() != MMA8451_RANGE_8_G)
    mma.setRange(MMA8451_RANGE_8_G);
  Serial.println("MMA8451 Initialized.");
}

// タイマー割り込み設定
void SetupTimer()
{
  // Timer0初期化
  while (!ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, Timer0Handler));
  Serial.println("ITimer0 Initialized.");
}

// シリアル読み込み
void ReadSerial()
{
  while (Serial.available()) 
  {
    // 読み込み
    String str = Serial.readString();
    // エコーバック
    Serial.println(str);
  }
}

// 加速度読み込み
void ReadAcceleration()
{
  if(canReadAcceleration)
  {
    mma.read();
    Serial.print(mma.x_g); Serial.print(",");
    Serial.print(mma.y_g); Serial.print(",");
    Serial.print(mma.z_g); Serial.println();
    canReadAcceleration = false;
  }
}

void setup() 
{
  // ピン初期化
  SetupPin();
  blink(500, 4);
  // シリアル初期化
  SetupSerial();
  blink(100, 10);
  // 加速度センサー初期化
  SetupAcceleration();
  // タイマー割り込み設定
  SetupTimer();
  // 処理開始
  blink(500, 6);
  digitalWrite(LED_BUILTIN, true);
}

void loop() 
{
  // シリアル読み込み
  ReadSerial();
  // 加速度読み込み
  ReadAcceleration();
}