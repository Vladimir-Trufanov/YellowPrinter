/** Esp32-2432S028R CYD                               *** YellowPrinter.ino ***
 *  
 *                           ПРИЁМНИК СООБЩЕНИЙ НА CYD - ДЕШЁВОМ ЖЕЛТОМ ДИСПЛЕЕ
 *        (железо и программа на CYD, которые принимают и показывают сообщения, 
 *                поступающие через ESP_NOW или по последовательному интерфейсу    
 * 
 * v2.0.1, 22.08.2026                                 Автор:      Труфанов В.Е.
 * Copyright © 2026 tve                               Дата создания: 13.07.2026
**/

// Определяем пин контрольного светодиода
#define LED_BUILTIN 16 // зеленый на CYD

// Определяем глобальную переменную counter, которая будет действовать как общий ресурс. 
// Две задачи - task1 и task2 могут обращаться к переменной counter. Однако, поскольку 
// это общий ресурс, задачи выполняются параллельно, нужен мьютекс для предотвращения 
// конфликтов
int counter = 0;  

#include <esp_now.h>
#include <ESP.h>

/***
// Готовим к использованию сторожевой таймер
#include <esp_task_wdt.h>
int WDT_TIMEOUT = 5; // WDT Timeout in seconds
*///

#include "inimem.h"
#include "WiFiOTA.h"
#include "yp_NVS.h"
#include "yp_ESPNOW.h"

/***
#include "spriteMain.h"
#include "TouchPress.h"
*///

/*
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
      memset(CtrlMessage.line,'\0',smLINESIZE); 
      memcpy(&CtrlMessage, incomingData, len);
      messCalc++;
      messBool=false;
      / *
      Serial.printf("\nTransmitter MAC Address: %02X:%02X:%02X:%02X:%02X:%02X \n", 
        info->src_addr[0], info->src_addr[1], info->src_addr[2], info->src_addr[3], info->src_addr[4], info->src_addr[5]);    
      * /
      Serial.print("CtrlMessage.line: "); Serial.println(CtrlMessage.line);
      / *
      Serial.printf("Длительность messageReceived(): %d ms\n\n", duration * portTICK_PERIOD_MS);
      * /
      xSemaphoreGive (messMutex);  
    }
    vTaskDelay(64);
  }
}
*/

// ****************************************************************************
// *                                 setup                                    *
// ****************************************************************************
void setup() 
{
  Serial.begin(115200);
  delay(3000);
  learnRestart();
  WiFi.mode(WIFI_STA);

  //iniWiFi(); 
  //iniOTA(); 
    
  //pinMode (LED_BUILTIN, OUTPUT);
 
  // Создаем объекты мьютексов
  //xMutex     = xSemaphoreCreateMutex();  
  messMutex  = xSemaphoreCreateMutex();  
  //touchMutex = xSemaphoreCreateMutex();  

  /*
  if (esp_now_init() == ESP_OK) 
  {
    Serial.println("ESPNow Init success");
  }
  else 
  {
    Serial.println("ESPNow Init fail");
    return;
  }
  */
  iniESPNOW();
  //esp_now_register_recv_cb(messageReceived);

  /***
  tft.init();
  tft.setRotation(1);      
  tft.fillScreen(TFT_NAVY);
  tft.setTextWrap(false, false);               // отключили перенос текста и по горизонтали и по вертикали 
  tft.setTextColor(TFT_WHITE,TFT_BLACK,true);  // определили цвет текста с прозрачным фоном  
  // Загружаем шрифт в память
  #ifdef FontFromFile
    tft.loadFont(HuaweiSans16);   
  #else
    tft.loadFont("HuaweiSans16");   
  #endif
 
  #ifdef FontFromSPIFFS
  // Инициализируем SPIFFS
  iniSPIFFS();
  #endif
  
  // Cоздаем задачи
  
  
  // Приём сообщения от внешнего контроллера-передатчика
  xTaskCreatePinnedToCore 
  (
    taskMain,       // Function to implement the task
    "taskMain",     // Name of the task
    4096,           // Stack size in words
    NULL,           // Task input parameter
    7,              // Priority of the task
    NULL,           // Task handle.
    0               // Core where the task should run
  );
  // текущей позиции touchscreen (на сенсорной панели)
  xTaskCreatePinnedToCore 
  (
    taskTouchscreen,      
    "taskTouchscreen",    
    4096,           // Stack size in words
    NULL,           // Task input parameter
    8,              // Priority of the task
    NULL,           // Task handle.
    0               // Core where the task should run
  );
   
  xTaskCreatePinnedToCore(
    vCheckFlagTask, // Task function
    "CheckFlags",   // Task name
    1024,           // Stack size
    NULL,           // Parameters passed to the task function
    9,              // Priority
    NULL,           // Task handle
    0
  );
  
   
  //tft.fillRect(0, 0, 16, 208, TFT_WHITE);
  / **
  Рисуем сглаженный (anti-aliased) прямоугольник с закруглёнными углами. 
  
  Синтаксис
  void drawSmoothRoundRect(int32_t x, int32_t y, int32_t r, int32_t ir, int32_t w, int32_t h, uint32_t fg_color, uint32_t bg_color = 0x00FFFFFF, uint8_t quadrants = 0xF);
  
  Параметры
  x, y — координаты верхнего левого угла прямоугольника на экране.
  r — радиус скругления вертикальных сторон.
  ir — радиус скругления горизонтальных сторон.
  w, h — ширина и высота прямоугольника соответственно.
  fg_color — цвет контура фигуры (передний план).
  bg_color — цвет фона (опционально, по умолчанию — белый 0x00FFFFFF).
  quadrants — битовая маска, которая определяет, какие части прямоугольника будут сглажены (по умолчанию рисуются все четыре дуги).

  В отличие от обычной функции drawRoundRect(), которая просто рисует контур, 
  drawSmoothRoundRect() создаёт более плавный и аккуратный вид за счёт сглаживания пикселей на границах фигуры. 
  
  Пример
  tft.drawSmoothRoundRect(10, 70, 10, 5, 50, 90, TFT_RED, TFT_BLACK);
  Здесь прямоугольник будет нарисован в точке (10, 70), с вертикальным радиусом 10, 
  горизонтальным 5, шириной 50 и высотой 90. Цвет контура — красный, фон — чёрный. 
  
  Особенности
  Сглаживание делает края фигуры плавными, устраняя «лесенку», что особенно заметно на текстах или иконках.
  Маска quadrants позволяет контролировать, какие дуги (верхняя, нижняя, левая, правая) будут сглажены.
  Это полезно для создания сложных эффектов.
  Если вам нужно не просто нарисовать контур, а заполнить фигуру цветом, в библиотеке есть родственный метод fillSmoothRoundRect() 
  ** /
  //tft.drawSmoothRoundRect(0, 26, 4, 8, 10, 30, TFT_RED, TFT_WHITE);
  *///
}

// ****************************************************************************
// *                                 loop                                     *
// ****************************************************************************
uint16_t iLoop=0;
static char taskList[1024]; 
void loop() 
{
  iLoop++;
  TickType_t start = xTaskGetTickCount();

  //ArduinoOTA.handle();

  // Считываем с последовательного порта целое число
  // (так как в зависимости от окружения за целым числом может следовать нулевое значение,
  // то отсекаем 0)  
  if (Serial.available() > 0) 
  {
    // 22/08/2026 Этот фрагмент кода проскакиваем дважды !!!
    int ii=Serial.parseInt();
    if (ii>0) inumber=ii;
    Serial.println("\n*********************************");
    Serial.print("Команда: "); Serial.println(inumber);
    Serial.println  ("*********************************\n");
  }

  /*
  // Мигаем зеленой лампочкой
  digitalWrite (LED_BUILTIN, HIGH);  
  vTaskDelay(1000);
  digitalWrite (LED_BUILTIN, LOW);   
  vTaskDelay(936);
  */
  
  // Если команда, то
  if (inumber==fOTA)
  {
    Serial.println("Переходим в режим OTA и отключаем ESPNOW");
    deiESPNOW();
    // Сбрасываем значение индикатора
    inumber=-1;  
  }
  
  // Если команда, то
  if (inumber==fWORK)
  {
    Serial.println("Переходим в рабочий режим и ВКЛЮЧАЕМ ESPNOW");
    iniESPNOW();
    // Сбрасываем значение индикатора
    inumber=-1;  
  }

  // Завершаем работу основного цикла
  TickType_t duration = xTaskGetTickCount() - start;
  //Serial.printf("Длительность loop(): %d ms\n", duration * portTICK_PERIOD_MS);
  // Отмечаем завершение цикла Loop для сторожевого таймера
  flag[fLoop] = 1;
  // Выполняем задержку на сборку мусора
  vTaskDelay(64);

  /*  
  // Имитируем зависание микроконтроллера с помощью опознанного числа,
  // принятого в последовательном порту
  if (inumber == fLoop) MimicMCUhangEvent("Loop");   
  // ---Имитируем зависание микроконтроллера с помощью опознанного числа,
  // ---принятого в последовательном порту

  / **
  Формируем читаемый отчёт со списком всех текущих задач и их состоянием с помощью  
  функции vTaskList(char *pcWriteBuffer) - в FreeRTOS она предназначена для отладки  

  Параметры функции:
  pcWriteBuffer — указатель на буфер в памяти, куда функция запишет сформированную таблицу 
  в текстовом (ASCII) виде. Предполагается, что размер буфера достаточен для всего отчёта. 
  Ориентировочно на одну задачу требуется около 40 байт. 

  Содержимое отчёта:
  Для каждой задачи в таблице выводится строка с полями:
  - Имя задачи, это cтрока, присвоенная задаче при создании (второй аргумент xTaskCreate()). 
    Может повторяться для разных экземпляров одной задачи, но уникальный номер задачи в системе будет разным.
  - Состояние, это однобуквенный код: B Blocked — задача заблокирована (например, ждёт события или таймера);
    R Ready — задача готова к выполнению и ждёт своего шанса; D Deleted — задача была удалена (через vTaskDelete()), 
    но ещё не освободила ресурсы и ждёт очистки; S Suspended — задача приостановлена (например, с помощью vTaskSuspend());
    X Executed (Running) — задача выполняется в данный момент.
  - Приоритет, это текущий приоритет задачи. Чем значение выше, тем приоритет выше.
  - Максимальный объём стека (Stack HWM) — это максимальное количество байт стека, 
    которое задача использовала за всё время своего существования. 
    Меньшее значение означает, что задача чаще приближалась к переполнению стека.
  - Уникальный номер задачи (Task Number) Идентификатор задачи, назначенный ядром FreeRTOS.
  - Адрес начала стека - это указатель на начало области памяти, выделенной под стек задачи. 

  Важные замечания:
  - Доступность функции. Чтобы vTaskList была доступна, в конфигурационном файле FreeRTOSConfig.h должны быть определены 
    в 1 три макроса: configUSE_TRACE_FACILITY, configUSE_STATS_FORMATTING_FUNCTIONS и INCLUDE_vTaskSuspend. 
  - Отключение прерываний. Во время выполнения функция отключает прерывания, что может повлиять на работу системы.
    Поэтому её не рекомендуется использовать в рабочем, производственном коде — только для отладки. 
  - Реализация. Внутри vTaskList вызывает uxTaskGetSystemState(), которая получает сырые данные 
    о состоянии всех задач, а затем форматирует их в читаемую таблицу. 
  - Зависимость от sprintf(). Функция использует стандартную функцию C sprintf(), что может увеличить 
    размер кода, потреблять стек и давать разные результаты на разных платформах. В некоторых демо-проектах 
    FreeRTOS есть альтернативная, более компактная реализация sprintf(). 
  - Рекомендация для продакшена. Если вам нужна «сырая» статистика для анализа, лучше напрямую вызывать 
    uxTaskGetSystemState(), а не форматировать его через vTaskList(). 
  ** /
  if (inumber == 207)
  {
    vTaskList(taskList);
    Serial.println(taskList);
    // Сбрасываем значение индикатора
    inumber=-1;  
  }
  //getheap("Цикл пройден ");
  */
}

/*
void vCheckFlagTask(void* pvParameters) 
{
  for ( ;; )
  {
    // Сбрасываем флаги и "пинаем сторожевую собаку" (fLoop=1, fmessageReceived=2)
    if (flag[fLoop] == 1 && flag[ftaskMain] == 1 && flag[ftaskTouchscreen] == 1) 
    {
      flag[fLoop] = 0;
      flag[ftaskMain] = 0;
      flag[ftaskTouchscreen] = 0;
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
*/

// ****************************************************** YellowPrinter.ino ***
