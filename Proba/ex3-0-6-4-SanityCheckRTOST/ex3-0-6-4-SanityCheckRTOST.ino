/** Arduino-Esp32-CAM                    *** ex3-0-6-4-SanityCheckRTOST.ino ***
 * 
 *                                  Проверка работоспособности нескольких задач
 *         для плат ESP32 в версии библиотеки от Espressif Systems версии 3.0.6
 *                                        (на контроллере AI-Thinker ESP32-CAM)
**/

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#include <esp_task_wdt.h>

int WDT_TIMEOUT = 5;          // WDT Timeout in seconds

void vTask1(void *pvParameters);
void vTask2(void *pvParameters);
void vTask3(void *pvParameters);
void vTask4(void *pvParameters);
//void vTaskIdle(void *pvParameters);

int flag[] = {0, 0, 0, 0};

// Определяем число, которое будет считываться в основном цикле
// с последовательного порта
volatile int inumber;


void setup() 
{
   Serial.begin(115200);

   xTaskCreatePinnedToCore(
      vTask1,                 // Task function
      "Task1",                // Task name
      1024,                   // Stack size
      NULL,                   // Parameters passed to the task function
      1,                      // Priority
      NULL,                   // Task handle
      ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
      vTask2,                 // Task function
      "Task2",                // Task name
      1024,                   // Stack size
      NULL,                   // Parameters passed to the task function
      1,                      // Priority
      NULL,                   // Task handle
      ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
      vTask3,                 // Task function
      "Task3",                // Task name
      1024,                   // Stack size
      NULL,                   // Parameters passed to the task function
      2,                      // Priority
      NULL,                   // Task handle
      ARDUINO_RUNNING_CORE);

    xTaskCreatePinnedToCore(
      vCheckFlagTask,         // Task function
      "CheckFlags",           // Task name
      1024,                   // Stack size
      NULL,                   // Parameters passed to the task function
      3,                      // Priority
      NULL,                   // Task handle
      ARDUINO_RUNNING_CORE);
}


// Имитируем событие зависания процессора
void MimicMCUhangEvent(String NameTask)
{
   while (true)
   {
      Serial.print(NameTask);
      Serial.println(": зависание процессора!!!");
   }
}


int it=0;
void loop() 
{
  it++;
  Serial.print("it = "); Serial.println(it);

   if (Serial.available() > 0) 
   {
      int ii=Serial.parseInt();
      if (ii>0) inumber=ii;
      delay(100);
   }
  flag[3] = 1;

      // Имитируем зависание микроконтроллера с помощью опознанного числа,
      // принятого в последовательном порту
      if (inumber == 4) MimicMCUhangEvent("Loop");   


  vTaskDelay(1000);
}

void vTask1(void* pvParameters) 
{
   for (;;)
   {
      Serial.println("Task1");
      flag[0] = 1;
      
      // Имитируем зависание микроконтроллера с помощью опознанного числа,
      // принятого в последовательном порту
      if (inumber == 1) MimicMCUhangEvent("Task1");   

      vTaskDelay(100/portTICK_PERIOD_MS);
   }
}

void vTask2(void* pvParameters) 
{
   for ( ;; )
   {
      Serial.println("Task2");
      // Mimic the MCU hang event using Serial port
      flag[1] = 1;
      // Имитируем зависание микроконтроллера с помощью опознанного числа,
      // принятого в последовательном порту
      if (inumber == 2) MimicMCUhangEvent("Task2");   
      vTaskDelay(100 / portTICK_PERIOD_MS);
   }
}

void vTask3(void* pvParameters) 
{
   for ( ;; )
   {
      flag[2] = 1;
            // Имитируем зависание микроконтроллера с помощью опознанного числа,
      // принятого в последовательном порту
      if (inumber == 3) MimicMCUhangEvent("Task3");   

      vTaskDelay(100 / portTICK_PERIOD_MS);
   }
}


void vCheckFlagTask(void* pvParameters) 
{
   for ( ;; )
   {
      // Check all flags if True then kick the dog
      if (flag[0] == 1 && flag[1] == 1 && flag[2] == 1 && flag[3] == 1) 
      {
         // Reset all flags
         flag[0] = 0;
         flag[1] = 0;
         flag[2] = 0;
         flag[3] = 0;
         // Kick the dog
         WDT_TIMEOUT = 5;
      }
      else 
      {
         WDT_TIMEOUT --;
         if (WDT_TIMEOUT == 0) 
         {
            ESP.restart();
         }
      }
      vTaskDelay(1000 / portTICK_PERIOD_MS);
   }
}

// ****************************************** ex3-0-6-4-SanityCheckRTOS.ino ***
