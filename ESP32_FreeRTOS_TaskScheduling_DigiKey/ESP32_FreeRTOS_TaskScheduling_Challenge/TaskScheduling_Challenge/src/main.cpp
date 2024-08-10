#include <Arduino.h>
#include <stdint.h>

#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif
//<---- ------------------------------------------------------------------ ---->
//<---- ------------------------------------------------------------------ ---->
char recNum_c[10];
uint8_t recNum_i;
//<---- ------------------------------------------------------------------ ---->
//<---- ------------------------------------------------------------------ ---->
void setup()
{
  Serial.begin(300);
  Serial.println("<---- Multi-task LED Demo ---->");
  Serial.println("<---- Enter a number in milliseconds to change the LED delay ---->");
}
//<---- ------------------------------------------------------------------ ---->
void loop()
{
  if(Serial.available())
  {
  }
}
//<---- ------------------------------------------------------------------ ---->