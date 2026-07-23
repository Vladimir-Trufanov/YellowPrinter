/** Esp32-2432S028R CYD                               *** YellowPrinter.ino ***
 *  
 *                           ПРИЁМНИК СООБЩЕНИЙ НА CYD - ДЕШЁВОМ ЖЕЛТОМ ДИСПЛЕЕ
 *        (железо и программа на CYD, которые принимают и показывают сообщения, 
 *                поступающие через ESP_NOW или по последовательному интерфейсу    
 * 
 * v1.0.3, 22.07.2026                                 Автор:      Труфанов В.Е.
 * Copyright © 2026 tve                               Дата создания: 13.07.2026
**/

// Определяем пин контрольного светодиода
// #define LED_BUILTIN 33    // на ESP32-CAM
#define LED_BUILTIN 16       // зеленый на CYD

// Определяем объект мьютекса - дескриптор (во FreeRTOS и мьютекс, и семафор реализованы
// как обычные совместно используемые подпрограммы. Это связано со сходством между обеими 
// концепциями. Дескриптор семафора может использоваться для создания семафоров любого 
// типа или мьютекса. xMutex это только дескриптор, инициализированный NULL значением. 
// Пока его невозможно использовать, это будет сделано в setup() 
// с помощью xSemaphoreCreateMutex())
SemaphoreHandle_t    xMutex = NULL;  
SemaphoreHandle_t messMutex = NULL;  

// Определяем мьютекс, который будет связан с критической секцией
// и проинициализируем его (то есть разблокируем для дальнейшего захвата)
//portMUX_TYPE taskMux = portMUX_INITIALIZER_UNLOCKED; 

// Определяем глобальную переменную counter, которая будет действовать как общий ресурс. 
// Две задачи - task1 и task2 могут обращаться к переменной counter. Однако, поскольку 
// это общий ресурс, задачи выполняются параллельно, нужен мьютекс для предотвращения 
// конфликтов
int counter = 0;  // A shared variable

#include <esp_now.h>
#include <WiFi.h>
#include <ESP.h>
#include <SPIFFS.h>
#include "inimem.h"

#include "spriteMain.h"
TSprite_Main ypsMain;

typedef struct message 
{
  char line[smLINESIZE];
} message;
message CtrlMessage;    // сообщение контроллера
message CYD_message;    // сообщение для дисплея CYD

uint16_t messCalc=0;
bool     messBool;  

void messageReceived(const esp_now_recv_info *info, const uint8_t* incomingData, int len)
{
  messBool=true;  
  while (messBool) 
  {
    // Как только захватили мьютекс, выполняем свою работу
    if (xSemaphoreTake (messMutex, portMAX_DELAY)) 
    {
      TickType_t start = xTaskGetTickCount();
      memset(CtrlMessage.line,'\0',smLINESIZE); 
      memcpy(&CtrlMessage, incomingData, len);
      Serial.printf("Transmitter MAC Address: %02X:%02X:%02X:%02X:%02X:%02X \n\r", 
        info->src_addr[0], info->src_addr[1], info->src_addr[2], info->src_addr[3], info->src_addr[4], info->src_addr[5]);    
      //Serial.print("Message: ");
      //Serial.println(CtrlMessage.line);
      //Serial.println();
      //ypsMain.View(CtrlMessage.line);
      messCalc++;
      messBool=false;
      xSemaphoreGive (messMutex);  
      TickType_t duration = xTaskGetTickCount() - start;
      Serial.printf("Длительность messageReceived(): %d ms\n", duration * portTICK_PERIOD_MS);
    }
    //delay(64);
    vTaskDelay(64);
  }

  /*
  //memcpy(&CtrlMessage, incomingData, sizeof(CtrlMessage));
  memset(CtrlMessage.line,'\0',smLINESIZE); 
  memcpy(&CtrlMessage, incomingData, len);
  Serial.printf("Transmitter MAC Address: %02X:%02X:%02X:%02X:%02X:%02X \n\r", 
    info->src_addr[0], info->src_addr[1], info->src_addr[2], info->src_addr[3], info->src_addr[4], info->src_addr[5]);    
  Serial.print("Message: ");
  Serial.println(CtrlMessage.line);
  Serial.println();
  ypsMain.View(CtrlMessage.line);
  */
  
}

// ============================================================================
void setup() 
{
  Serial.begin(115200);
  delay(3000); // uncomment if your serial monitor is empty

  /*
  const TickType_t Delay500   = pdMS_TO_TICKS(500); 
  const TickType_t Delay10000 = pdMS_TO_TICKS(10000); 
  const TickType_t Delay64    = pdMS_TO_TICKS(64); 
  Serial.print("Delay500   = "); Serial.println(Delay500);
  Serial.print("Delay10000 = "); Serial.println(Delay10000);
  Serial.print("Delay64    = "); Serial.println(Delay64);
  vTaskDelay(Delay10000);     // Задержка
  */

  getheap("setup        ");
  
  WiFi.mode(WIFI_STA);
  if (esp_now_init() == ESP_OK) 
  {
    Serial.println("ESPNow Init success");
  }
  else 
  {
    Serial.println("ESPNow Init fail");
    return;
  }
  esp_now_register_recv_cb(messageReceived);


  tft.init();
  tft.setRotation(1);      
  tft.fillScreen(TFT_NAVY);
  
  // инициализация SPIFFS
  if (!SPIFFS.begin()) 
  {
    while (1) yield();
  }  

   pinMode (LED_BUILTIN, OUTPUT);
   // Создаем объект мьютекса - мьютекс
   xMutex = xSemaphoreCreateMutex();  
   messMutex = xSemaphoreCreateMutex();  
   // Cоздаем задачи, привязывая их к ядру 0
   xTaskCreatePinnedToCore 
   (
      task1,     // Function to implement the task
      "task1",   // Name of the task
      1024,      // Stack size in words
      NULL,      // Task input parameter
      10,        // Priority of the task
      NULL,      // Task handle.
      0          // Core where the task should run
   );

   xTaskCreatePinnedToCore 
   (
      task2,     // Function to implement the task
      "task2",   // Name of the task
      1024,      // Stack size in words
      NULL,      // Task input parameter
      10,        // Priority of the task
      NULL,      // Task handle.
      0          // Core where the task should run
   );

   xTaskCreatePinnedToCore 
   (
      taskMain,     // Function to implement the task
      "taskMain",   // Name of the task
      4096,      // Stack size in words
      NULL,      // Task input parameter
      15,        // Priority of the task
      NULL,      // Task handle.
      0          // Core where the task should run
   );
}

uint16_t i=0;
//char lineIn[smLINESIZE];    // буфер входного сообщения
//char chi[] = "Число i = ";

// ============================================================================ 
void loop() 
{
  //portENTER_CRITICAL(&taskMux);  // lock the mutex (busy waiting)
  TickType_t start = xTaskGetTickCount();
  digitalWrite (LED_BUILTIN, HIGH);  
  vTaskDelay(1000);
  digitalWrite (LED_BUILTIN, LOW);   
  vTaskDelay(1000);
  getheap("Цикл пройден ");
  i++;
  TickType_t duration = xTaskGetTickCount() - start;
  Serial.printf("Длительность loop(): %d ms\n", duration * portTICK_PERIOD_MS);
  //portEXIT_CRITICAL (&taskMux);   // unlock the mutex
}

// ============================================================================
// Сначала в задаче task1 пытаемся захватить и заблокировать мьютекс xMutex с помощью функции 
// xSemaphoreTake(xMutex,portMAX_DELAY). Функции передаются два параметра: дескриптор мьютекса 
// и значение таймаута. Здесь используется portMAX_DELAY макрос, который соответствует 
// неопределенной задержке. Это означает, что task1 будет пытаться получить блокировку 
// на неопределенный срок, пока не добьется успеха. Когда получаем блокировку, 
// то печатаем некоторую информацию и увеличиваем значение counter на 1.
// Затем печатаем значение счетчика и ждем 1 секунду, прежде чем снять 
// блокировку мьютекса с помощью вызова xSemaphoreGive(xMutex), отдаем мьютекс и ждем еще 800 
// миллисекунд, прежде чем повторить всю операцию.
void task1 (void *pvParameters) 
{
  while (1) 
  {
    // Как только захватили мьютекс, выполняем свою работу
    if (xSemaphoreTake (xMutex, portMAX_DELAY)) 
    {  
      //Serial.print ("Task 1: Mutex взят задачей ");
      //Serial.println (xTaskGetTickCount());
      counter = counter + 1;  
      Serial.print ("Task 1: Counter = ");
      Serial.println (counter);
      vTaskDelay(1000);
      xSemaphoreGive (xMutex);  
      vTaskDelay(800);
    }
  }
  vTaskDelay(1000);
}

// ============================================================================
// В task2 конкурируем за ту же counter переменную. Пытаемся заблокировать xMutex 
// с таймаутом в 200 миллисекунд, указанным как 200*portTICK_PERIOD_MS. Если захватить
// блокировку не получается в течение этого периода времени, прекращаем попытки 
// и вместо этого печатает сообщение. 
//
// !!! Блокировка task2 может быть установлена, только если она находится в состоянии 
// разблокировки или, другими словами, не заблокирована task1. Поскольку для освобождения 
// мьютекса task1 требуется 1 секунда, task2 придется подождать не менее 1 секунды, 
// прежде чем он сможет получить блокировку. 
//
// Это будет видно в выводе на последовательный монитор
void task2 (void *pvParameters) 
{
   while (1) 
   {
      if (xSemaphoreTake (xMutex, (200 * portTICK_PERIOD_MS))) 
      { 
         //Serial.print ("Task 2: Mutex взят задачей ");
         //Serial.println (xTaskGetTickCount());
         counter = counter + 1000;
         //Serial.print ("Task 2: Counter = ");
         //Serial.println (counter);
         xSemaphoreGive (xMutex);  
      }
      else 
      {  
         //Serial.print ("Task 2: Mutex не захвачен ");
         //Serial.println (xTaskGetTickCount());
      }
      vTaskDelay(200);
  }
}

uint16_t copyCalc=195;  // !=0

void taskMain (void *pvParameters) 
{
  while (1) 
  {
    if (xSemaphoreTake(messMutex, (200 * portTICK_PERIOD_MS))) 
    { 
      //Serial.print("copyCalc==messCalc: "); Serial.print(copyCalc); Serial.print("="); Serial.println(messCalc);
      if (!(copyCalc==messCalc))
      {
        memcpy(CYD_message.line, CtrlMessage.line, smLINESIZE);
        Serial.println("***");
        Serial.println(CtrlMessage.line);
        Serial.println(CYD_message.line);
        Serial.println("***");
        ypsMain.View(CYD_message.line);

        // memset(CtrlMessage.line,'\0',smLINESIZE); 
        // memcpy(&CtrlMessage, incomingData, len);
        // memcpy(&CtrlMessage.line, CtrlMessage.line, smLINESIZE);

        copyCalc=messCalc;
      }
      xSemaphoreGive(messMutex);  
    }
    else 
    {  
      //Serial.print ("Task 2: Mutex не захвачен ");
      //Serial.println (xTaskGetTickCount());
    }
    vTaskDelay(64);
  }
}

// ****************************************************** YellowPrinter.ino ***
