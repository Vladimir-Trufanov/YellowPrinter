/** Esp32-2432S028R CYD                               *** YellowPrinter.ino ***
 *  
 *                           ПРИЁМНИК СООБЩЕНИЙ НА CYD - ДЕШЁВОМ ЖЕЛТОМ ДИСПЛЕЕ
 *        (железо и программа на CYD, которые принимают и показывают сообщения, 
 *                поступающие через ESP_NOW или по последовательному интерфейсу    
 * 
 * v1.0.6, 26.07.2026                                 Автор:      Труфанов В.Е.
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

// Определяем глобальную переменную counter, которая будет действовать как общий ресурс. 
// Две задачи - task1 и task2 могут обращаться к переменной counter. Однако, поскольку 
// это общий ресурс, задачи выполняются параллельно, нужен мьютекс для предотвращения 
// конфликтов
int counter = 0;  

#include <esp_now.h>
#include <WiFi.h>
#include <ESP.h>
#include <SPIFFS.h>
#include "inimem.h"

// Готовим к использованию сторожевой таймер
#include <esp_task_wdt.h>
int WDT_TIMEOUT = 5; // WDT Timeout in seconds

// Определяем число, которое будет считываться в основном цикле
// с последовательного порта для иммитации зависания и других действий
volatile int inumber=-1;
// Флаги контрольных участков сторожевого таймера
#define fLoop      1   // 1 => loop();
#define ftaskMain  2   // 2 => taskMain()
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
      //Serial.printf("Длительность messageReceived(): %d ms\n", duration * portTICK_PERIOD_MS);
    }
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
   // Cоздаем задачи
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
static char taskList[1024]; 
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
  //Serial.printf("Длительность loop(): %d ms\n", duration * portTICK_PERIOD_MS);
  // Отмечаем завершение цикла Loop для сторожевого таймера
  flag[fLoop] = 1;
  // Имитируем зависание микроконтроллера с помощью опознанного числа,
  // принятого в последовательном порту
  if (inumber == fLoop) MimicMCUhangEvent("Loop");   
  // ---Имитируем зависание микроконтроллера с помощью опознанного числа,
  // ---принятого в последовательном порту

/*
 * 
Функция vTaskList(char *pcWriteBuffer) в FreeRTOS предназначена для отладки: она формирует читаемый отчёт со списком всех текущих задач и их состоянием. 
docs.espressif.com
docs.espressif.com
microsin.net
Параметры функции
pcWriteBuffer — указатель на буфер в памяти, куда функция запишет сформированную таблицу в текстовом (ASCII) виде. Предполагается, что размер буфера достаточен для всего отчёта. Ориентировочно на одну задачу требуется около 40 байт. 
docs.espressif.com
docs.espressif.com
Содержимое отчёта
Для каждой задачи в таблице выводится строка с полями:
Поле  Что означает
Имя задачи  Строка, присвоенная задаче при создании (второй аргумент xTaskCreate()). Может повторяться для разных экземпляров одной задачи, но уникальный номер задачи в системе будет разным.
Состояние Однобуквенный код:
B Blocked — задача заблокирована (например, ждёт события или таймера).
R Ready — задача готова к выполнению и ждёт своего шанса.
D Deleted — задача была удалена (через vTaskDelete()), но ещё не освободила ресурсы и ждёт очистки.
S Suspended — задача приостановлена (например, с помощью vTaskSuspend()).
X Executed (Running) — задача выполняется в данный момент.
Приоритет Текущий приоритет задачи. Чем значение выше, тем приоритет выше.
Максимальный объём стека (Stack HWM)  Показатель «высокого водного mark» (HWM) — это максимальное количество байт стека, которое задача использовала за всё время своего существования. Меньшее значение означает, что задача чаще приближалась к переполнению стека.
Уникальный номер задачи (Task Number) Идентификатор задачи, назначенный ядром FreeRTOS.
Адрес начала стека  Указатель на начало области памяти, выделенной под стек задачи. 
kit-e.ru
docs.espressif.com
docs.espressif.com
openrtos.org
microsin.net
Важные замечания
Доступность функции. Чтобы vTaskList была доступна, в конфигурационном файле FreeRTOSConfig.h должны быть определены в 1 три макроса: configUSE_TRACE_FACILITY, configUSE_STATS_FORMATTING_FUNCTIONS и INCLUDE_vTaskSuspend. 
docs.espressif.com
kolegite.com
Отключение прерываний. Во время выполнения функция отключает прерывания, что может повлиять на работу системы. Поэтому её не рекомендуется использовать в рабочем, производственном коде — только для отладки. 
docs.espressif.com
docs.espressif.com
Реализация. Внутри vTaskList вызывает uxTaskGetSystemState(), которая получает сырые данные о состоянии всех задач, а затем форматирует их в читаемую таблицу. 
docs.espressif.com
docs.espressif.com
Зависимость от sprintf(). Функция использует стандартную функцию C sprintf(), что может увеличить размер кода, потреблять стек и давать разные результаты на разных платформах. В некоторых демо-проектах FreeRTOS есть альтернативная, более компактная реализация sprintf(). 
docs.espressif.com
Рекомендация для продакшена. Если вам нужна «сырая» статистика для анализа, лучше напрямую вызывать uxTaskGetSystemState(), а не форматировать его через vTaskList(). 
docs.espressif.com
Пример вывода
Name State Priority Stack Num
Print R 4 358 64
QConsB R 0 192 58
IDLE R 0 212 66
В этом примере задача Print выполняется (X), задача QConsB заблокирована (B), а IDLE готова к выполнению (R).
 */
  
  if (inumber == 3)
  {
    vTaskList(taskList);
    Serial.println(taskList);
    // Сбрасываем значение индикатора
    inumber=-1;  
  }
  getheap("Цикл пройден ");
}

void vCheckFlagTask(void* pvParameters) 
{
  for ( ;; )
  {
    // Сбрасываем флаги и "пинаем сторожевую собаку" (fLoop=1, fmessageReceived=2)
    if (flag[fLoop] == 1 && flag[ftaskMain] == 1) 
    {
      flag[fLoop] = 0;
      flag[ftaskMain] = 0;
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
    TickType_t start = xTaskGetTickCount();
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
    // Отмечаем завершение цикла задаси для сторожевого таймера
    flag[ftaskMain] = 1;
    TickType_t duration = xTaskGetTickCount() - start;
    //Serial.printf("Длительность taskMain(): %d ms\n", duration * portTICK_PERIOD_MS);
    // Если было введено число=fmessageReceived
    if (inumber == ftaskMain) MimicMCUhangEvent("taskMain");   

    vTaskDelay(64);
  }
}

// ****************************************************** YellowPrinter.ino ***
