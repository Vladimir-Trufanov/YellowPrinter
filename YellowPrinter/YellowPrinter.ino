/** Esp32-2432S028R CYD                               *** YellowPrinter.ino ***
 *  
 *                           ПРИЁМНИК СООБЩЕНИЙ НА CYD - ДЕШЁВОМ ЖЕЛТОМ ДИСПЛЕЕ
 *        (железо и программа на CYD, которые принимают и показывают сообщения, 
 *                поступающие через ESP_NOW или по последовательному интерфейсу    
 * 
 * v1.0.5, 25.07.2026                                 Автор:      Труфанов В.Е.
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

// Готовим к использованию сторожевой таймер
#include <esp_task_wdt.h>
int WDT_TIMEOUT = 5; // WDT Timeout in seconds

// Определяем число, которое будет считываться в основном цикле
// с последовательного порта для иммитации зависания
volatile int inumber;
// Флаги контрольных участков сторожевого таймера
#define fLoop             1   // 1 => loop();
#define fmessageReceived  2   // 2 => messageReceived()
int flag[] = {-1,0,0};   

#include "spriteMain.h"
TSprite_Main ypsMain;

typedef struct message 
{
  char line[smLINESIZE];
} message;
message CtrlMessage;    // сообщение контроллера
message CYD_message;    // сообщение для дисплея CYD

// Инициируем счетчик поступающих сообщений
uint16_t messCalc=0;   
// Готовим индикатор ожидания мьютекса и выборки поступающего сообщения
bool messBool;      
  
// ****************************************************************************
// *            Принять поступающее сообщение в захвате мьютекса              *
// ****************************************************************************
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
      //Serial.print("CtrlMessage.line: "); Serial.println(CtrlMessage.line); Serial.println();
      messCalc++;
      messBool=false;
      xSemaphoreGive (messMutex);  
      TickType_t duration = xTaskGetTickCount() - start;
      Serial.printf("Длительность messageReceived(): %d ms\n", duration * portTICK_PERIOD_MS);
    }
    // Отмечаем завершение функции для сторожевого таймера
    flag[fmessageReceived] = 1;
    // Если было введено число=fmessageReceived
    if (inumber == fmessageReceived) MimicMCUhangEvent("messageReceived");   
    vTaskDelay(64);
  }
}

// ****************************************************************************
// *                                 setup                                    *
// ****************************************************************************
void setup() 
{
  Serial.begin(115200);
  delay(300);
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
   
   xTaskCreatePinnedToCore(
      vCheckFlagTask,         // Task function
      "CheckFlags",           // Task name
      1024,                   // Stack size
      NULL,                  // Parameters passed to the task function
      16,                     // Priority
      NULL,                   // Task handle
      0
   );
}

// ****************************************************************************
// *                  Имитировать событие зависания процессора                *
// ****************************************************************************
void MimicMCUhangEvent(String NameTask)
{
  while (true)
  {
    Serial.print(NameTask);
    Serial.println(": зависание процессора!!!");
  }
}

// ****************************************************************************
// *                                 loop                                     *
// ****************************************************************************
uint16_t iLoop=0;
void loop() 
{
  // Считываем с последовательного порта целое число
  // (так как в зависимости от окружения за целым числом может следовать нулевое значение,
  // то отсекаем 0)  
  if (Serial.available() > 0) 
  {
    int ii=Serial.parseInt();
    if (ii>0) inumber=ii;
  }
  TickType_t start = xTaskGetTickCount();
  digitalWrite (LED_BUILTIN, HIGH);  
  vTaskDelay(1000);
  digitalWrite (LED_BUILTIN, LOW);   
  vTaskDelay(872);
  // Задержку на сборку мусора
  vTaskDelay(128);
  iLoop++;
  TickType_t duration = xTaskGetTickCount() - start;
  Serial.printf("Длительность loop(): %d ms\n", duration * portTICK_PERIOD_MS);
  // Отмечаем завершение цикла Loop для сторожевого таймера
  flag[fLoop] = 1;
  // Имитируем зависание микроконтроллера с помощью опознанного числа,
  // принятого в последовательном порту
  if (inumber == fLoop) MimicMCUhangEvent("Loop");   
  getheap("Цикл пройден ");
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
  if (inumber == 1) MimicMCUhangEvent("task1");   
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


void vCheckFlagTask(void* pvParameters) 
{
  for ( ;; )
  {
    // Сбрасываем флаги и "пинаем сторожевую собаку" (fLoop=1, fmessageReceived=2)
    if (flag[fLoop] == 1 && flag[fmessageReceived] == 1) 
    {
      flag[fLoop] = 0;
      flag[fmessageReceived] = 0;
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
    vTaskDelay(1000/portTICK_PERIOD_MS);
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
