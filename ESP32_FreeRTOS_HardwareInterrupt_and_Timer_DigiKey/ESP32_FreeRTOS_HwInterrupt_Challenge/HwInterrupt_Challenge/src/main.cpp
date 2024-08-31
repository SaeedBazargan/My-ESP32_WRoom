#include <Arduino.h>
#include <freertos/ringbuf.h>

#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif
//<---- ------------------------------------------------------------------ ---->
#define lengthOfNumber              5
#define led_pin                     LED_BUILTIN
//<---- ------------------------------------------------------------------ ---->
char recNum_c[lengthOfNumber];
uint8_t counterOfRecNumber = 0;
static int realNumber = 500;

static const uint16_t timer_divider = 800;   // Clock tick at 10 MHz (ESP32 Timer = 80MHz, So timer_divider = 800 it means 10 Sec)
static const uint64_t timer_max_count = 1000000;

volatile uint8_t recordCount = 0;
//<---- ------------------------------------------------------------------ ---->
static TaskHandle_t recordTask_Handle = NULL;
static RingbufHandle_t buffer_Handle = NULL;
static hw_timer_t *hal_timer = NULL;
//<---- ------------------------------------------------------------------ ---->
void recordSerial_Task(void* parameters);
void IRAM_ATTR timer_isr_handler();
//<---- ------------------------------------------------------------------ ---->
//<---- ------------------------------------------------------------------ ---->

void setup() 
{
  Serial.begin(300);
  Serial.println("<---- FreeRTOS Hardware Interrupt Demo ---->");
  Serial.println("<---- Enter some numbers to calculate the average! ---->");

  pinMode(led_pin, OUTPUT);

  vTaskDelay(1000 / portTICK_PERIOD_MS);

  buffer_Handle = xRingbufferCreate(10, RINGBUF_TYPE_BYTEBUF);
  
  xTaskCreatePinnedToCore(recordSerial_Task, "RECORD_TASK", 1024, NULL, 1, &recordTask_Handle, app_cpu);

  vTaskDelete(NULL);
}
//<---- ------------------------------------------------------------------ ---->
void loop() 
{}
//<---- ------------------------------------------------------------------ ---->
void recordSerial_Task(void* parameters)
{
  uint32_t ulNotifyValue;

  int* recMsg;

  while (1)
  {
    ulNotifyValue = ulTaskNotifyTake(pdTRUE, 0);
    if(ulNotifyValue != 0)
    {
      digitalWrite(led_pin, HIGH);
      Serial.println("Hello from Timer_ISR function");

      size_t receivedMessageSize;
      recMsg = (int*)xRingbufferReceiveUpTo(buffer_Handle, &receivedMessageSize, 0, 10);
      Serial.println(*recMsg);
    }

    if(Serial.available())
    {
      recNum_c[++counterOfRecNumber] = Serial.read();
      Serial.print(recNum_c[counterOfRecNumber]);

      if(counterOfRecNumber > 10)
      {
        Serial.println("Wrong Number!");
      }
      else if(recNum_c[counterOfRecNumber] == '\n')
      {
        realNumber = atoi(&recNum_c[1]);
        
        xRingbufferSend(buffer_Handle, &realNumber, sizeof(realNumber), 0);

        hal_timer = timerBegin(0, timer_divider, true);               // Create and Start timer (timer_number, divider, countUp)
        timerAttachInterrupt(hal_timer, &timer_isr_handler, true);    // Provide ISR to timer (timer, void (*function)(), edge)
        timerAlarmWrite(hal_timer, timer_max_count, false);           // At what count should ISR trigger (timer, count, autoreload)
        timerAlarmEnable(hal_timer);                                  // Allow ISR to trigger

        memset(recNum_c, 0, lengthOfNumber);
        counterOfRecNumber = 0;
      }
    }
  }
}
//<---- ------------------------------------------------------------------ ---->
void IRAM_ATTR timer_isr_handler()
{
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;

  vTaskNotifyGiveFromISR(recordTask_Handle, &xHigherPriorityTaskWoken);

  if(xHigherPriorityTaskWoken==pdTRUE)
  {
    portYIELD_FROM_ISR();
  }
}