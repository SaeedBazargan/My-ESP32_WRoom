#include <Arduino.h>
#include <stdint.h>

#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

#define msg_queue_length            5
#define lengthOfChar                50
#define led_pin                     BUILTIN_LED
//<---- --------------------------------------------------------- ---->
static QueueHandle_t Queue_1, Queue_2;

static TaskHandle_t Task_1 = NULL;
static TaskHandle_t Task_2 = NULL;
//<---- --------------------------------------------------------- ---->
char recChar_c[lengthOfChar];
uint8_t counterOfRecChar = 0;
//<---- --------------------------------------------------------- ---->
void startTask1(void* parameter);
void startTask2(void* parameter);
//<---- --------------------------------------------------------- ---->
void setup()
{
  Serial.begin(300);
  Serial.println("<---- FreeRTOS Queue Demo ---->");

  pinMode(led_pin, OUTPUT);

  vTaskDelay(1000 / portTICK_PERIOD_MS);

  Queue_1 = xQueueCreate(msg_queue_length, sizeof(int));
  Queue_2 = xQueueCreate(msg_queue_length, sizeof(int));

  xTaskCreatePinnedToCore(startTask1, "TASK_1", 1024, NULL, 1, &Task_1, app_cpu);
  // xTaskCreatePinnedToCore(startTask2, "TASK_2", 1024, NULL, 1, &Task_2, app_cpu);
}
//<---- --------------------------------------------------------- ---->
void loop()
{}
//<---- --------------------------------------------------------- ---->
void startTask1(void* parameter)
{
  int rec;
  
  while (1)
  {
    if(xQueueReceive(Queue_2, (void*)&rec, 0) == pdTRUE)
    {
      Serial.print("<---- Recieve from Queue ---->");
      Serial.println(rec);
    }

    if(Serial.available())
    {
      recChar_c[++counterOfRecChar] = Serial.read();
      Serial.print(recChar_c[counterOfRecChar]);

      if(counterOfRecChar > 49)
      {
        Serial.println(" Write Shorter than 50 words!");
        
        memset(recChar_c, 0, lengthOfChar);
        counterOfRecChar = 0;
      }
      else if(recChar_c[counterOfRecChar] == '\n')
      {
        Serial.print("your character is: ");
        Serial.println(&recChar_c[1]);

        if(memcmp(&recChar_c[1], "delay ", 6) == 0)
        {
          int tmp = atoi(&recChar_c[7]);
          
          if(xQueueSend(Queue_1, (void*)&tmp, 0) != pdTRUE)
          {
            Serial.print("you can not send delay time to Queue_1!");
          }
        }

        memset(recChar_c, 0, lengthOfChar);
        counterOfRecChar = 0;
      }
    }
  }
}
//<---- --------------------------------------------------------- ---->
void startTask2(void* parameter)
{}
