#include <Arduino.h>

#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

#define led_pin                     LED_BUILTIN
//<---- ------------------------------------------------------------------ ---->
static const uint16_t timer_divider = 80;   // Clock tick at 1 MHz (ESP32 Timer = 80MHz, So timer_divider = 80 it means 1 Sec)
static const uint64_t timer_max_count = 1000000;

static hw_timer_t *hal_timer = NULL;
//<---- ------------------------------------------------------------------ ---->
void IRAM_ATTR onTimer();
//<---- ------------------------------------------------------------------ ---->
//<---- ------------------------------------------------------------------ ---->

void setup() 
{
  pinMode(led_pin, OUTPUT);

  // Create and Start timer (timer_number, divider, countUp)
  hal_timer = timerBegin(0, timer_divider, true);

  // Provide ISR to timer (timer, void (*function)(), edge)
  timerAttachInterrupt(hal_timer, &onTimer, true);

  // At what count should ISR trigger (timer, count, autoreload)
  timerAlarmWrite(hal_timer, timer_max_count, true);

  //Allow ISR to trigger
  timerAlarmEnable(hal_timer);
}
//<---- ------------------------------------------------------------------ ---->
void loop()
{}
//<---- ------------------------------------------------------------------ ---->
void IRAM_ATTR onTimer()  // This ISR function executes when timer reaches max and resets
{
  uint8_t pinstate = digitalRead(led_pin);
  digitalWrite(led_pin, !pinstate);
}