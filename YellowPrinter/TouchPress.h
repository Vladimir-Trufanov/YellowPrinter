/** YellowPrinter-Esp32-Arduino                            *** TouchPress.h ***
 * 
 * Обнаружить нажатие на сенсорной панели и зафиксировать данные нажатия
 * 
 * v2.0.1, 27.08.2026                                 Автор:      Труфанов В.Е.
 * Copyright © 2026 tve                               Дата создания: 12.08.2026
**/

#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <XPT2046_Touchscreen.h>

#include "inimem.h"
#include "WiFiOTA.h"

// st - префикс спрайта spriteTouch - данных по нажатию на сенсорной панели 
#define stLINESIZE     80    // размер буфера одного сообщения
#define stWIDTH       304    // ширина спрайта сообщений 
#define stHEIGHT       16    // высота спрайта
#define stTOP         207    // позиция по вертикали размещения спрайта на экране 
#define stLEFT         17    // позиция по горизонтали

// Touchscreen pins
#define XPT2046_IRQ    36    // T_IRQ
#define XPT2046_MOSI   32    // T_DIN
#define XPT2046_MISO   39    // T_OUT
#define XPT2046_CLK    25    // T_CLK
#define XPT2046_CS     33    // T_CS

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);
TFT_eSprite spriteTouch = TFT_eSprite(&tft);

bool isTouchOTA=true;     // начат отсчет нажатий по запуску загрузки через OTA
uint8_t clickTouchOTA=0;  // еще не сделано первое последовательное нажатие из трех

class TSprite_Touch 
{
  public:

  TSprite_Touch(); 
  void View(int x, int y, int z, uint16_t touchCalc);

  private:

  char line[stLINESIZE];  // буфер cообщения
};

TSprite_Touch::TSprite_Touch() 
{
};

void TSprite_Touch::View(int x, int y, int z, uint16_t touchCalc)
{
  spriteTouch.setColorDepth(8);
  spriteTouch.createSprite(stWIDTH, stHEIGHT);
  if (spriteTouch.created())
  {
    // Заполняем буфер памяти, выделенный под спрайт, заданным цветом
    spriteTouch.fillSprite(TFT_BLACK);
    // Отключаем перенос текста и по горизонтали и по вертикали 
    spriteTouch.setTextWrap(false, false);
    // Определяем цвет текста с прозрачным фоном  
    spriteTouch.setTextColor(TFT_WHITE,TFT_BLACK,true); 
    // Загружаем шрифт в память спрайта
    #ifdef FontFromFile
      spriteTouch.loadFont(HuaweiSans16);   
    #else
      spriteTouch.loadFont("HuaweiSans16");   
    #endif
    // Формируем сообщение с данными о нажатии на сенсорную панель
    memset(line,'\0',stLINESIZE); 
    strcat(line,"x:"); 
    strcat(line,IntToChar(x)); 
    strcat(line," y:"); 
    strcat(line,IntToChar(y)); 
    strcat(line," z:"); 
    strcat(line,IntToChar(z)); 
    strcat(line," touchCalc = "); 
    strcat(line,IntToChar(touchCalc)); 

    spriteTouch.setCursor(0,0);
    spriteTouch.print(line);
    Serial.println(line);
    spriteTouch.pushSprite(stLEFT,stTOP);
    getheap("===> spriteTouch.pushSprite"); 

    // Выгружаем шрифт из памяти и удаляем спрайт
    spriteTouch.unloadFont();          
    spriteTouch.deleteSprite();
  }
  else
  {
    Serial.println("НЕ ПОЛУЧИЛОСЬ создать spriteTouch!");
  }
}

// Объект для работы с сенсорной панелью
TSprite_Touch ypsTouch;

void taskTouchscreen (void *pvParameters) 
{
  char mess[smLINESIZE];  // буфер входного сообщения
  int x, y, z;            // координаты точки касания (x,y) и силу нажатия (z)
  uint16_t touchCalc=0;   // счетчик нажатий на сенсорную панель

  // Запускаем SPI для сенсорного экрана и инициализируем экран
  touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin(touchscreenSPI);
  touchscreen.setRotation(1);

  while (1) 
  {
    // Фиксируем начало цикла задачи
    TickType_t start = xTaskGetTickCount();
    // Если НЕ команда "Перейти в режим перепрошивки по OTA"
    if (inumber!=fOTA)
    {
      // Проверяем, есть ли касание к сенсорному экрану, и выводим информацию о X, Y 
      // и давлении (Z) на TFT-дисплей и последовательный монитор
      if (touchscreen.tirqTouched() && touchscreen.touched()) 
      {
        // Изменяем счетчик
        touchCalc++;
        // Получаем данные по точке нажатия на сенсорном экране
        TS_Point p = touchscreen.getPoint();
        // Калибруем точку на сенсорном экране с помощью функции отображения на 
        // карте по фактической ширине и высоте
        /*
        Этот код — калибровка сенсорного экрана для проектов на платформе Arduino 
        (часто встречается на платах ESP32 с TFT-дисплеем, например, на «Cheap Yellow Display», CYD). 

        Функция map() преобразует «сырые» значения с сенсорного экрана в координаты пикселей дисплея, где: 
    
        p.x — значение, которое сенсор возвращает при касании по оси X. 200, 3700 — эмпирически найденные 
        границы диапазона «сырых» значений. При касании в крайних точках (слева и справа) сенсор выдаёт 
        примерно эти числа. Эти значения учитывают «мёртвую зону» у края экрана, смещение АЦП и 
        нелинейность сенсора;
    
        1, SCREEN_WIDTH — диапазон, в который функция «мапит» результат. То есть: если сенсор при касании 
        в правой границе экрана вернул 3700, map() пересчитает это в SCREEN_WIDTH (собственно, ширину 
        дисплея в пикселях). 
    
        Зачем это нужно. Сенсорный экран часто физически больше видимой области дисплея, 
        а его контроллер выдаёт значения за пределами видимой зоны. Функция map() «сжимает» 
        или «растягивает» диапазон сырых сенсорных данных до нужного диапазона пикселей 
        дисплея. Это позволяет логике работы с касаниями (например, проверка попадания 
        в кнопки) оперировать привычными пиксельными координатами.
    
        Важный нюанс. Числа 200, 3700 (и аналогично для оси Y: 240, 3800) не универсальны. Они 
        зависят от конкретной модели сенсора, способа подключения и его калибровки. Если касания 
        «съезжают» или работают некорректно, эти значения нужно найти экспериментально (нажать 
        в крайних точках и записать показания) и подставить свои в map().
    
        Аналогично для оси Y: y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);.
    
        Так что эти строки — не жёсткий стандарт, а рабочий шаблон, который нужно адаптировать 
        под вашу конкретную связку «экран + контроллер». 

        1. [Подключение сенсорного TFT дисплея к ESP32: Схема и пример кода]
        (https://voltiq.ru/esp32-touchscreen-display-connection/)
    
        2. [Ultimate Beginner’s Guide to the Cheap Yellow Display]
        (https://kafkar.com/projects/smart-home/mastering-the-cyd-your-ultimate-beginners-guide-to-the-cheap-yellow-display-esp32-2432s028r-using-platform-io/)
        */
        x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
        y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);
        z = p.z;
        // Выводим данные
        Serial.println("Есть touchscreen");
        ypsTouch.View(x,y,z,touchCalc);
      }
      // Размещаем данные для других задач
      /*
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
      */ 
      
      // Ловим трехкратные последовательные нажатия в строке локальных сообщений
      // [17,223]-[319,239] для переключения на загрузку через OTA 
      if ((x>=17) && (x<=319) && (y>=223) && (y<=239))
      {
        // Увеличиваем счетчик нажатий для OTA
        isTouchOTA=true; // идет отсчет нажатий по запуску загрузки через OTA
        clickTouchOTA++;
        // Проверяем и запускаем OTA
        if (clickTouchOTA >= 3)
        {
          inumber=fOTA;
          launchOTA();
        }
      }
      // Сбрасываем флаг загрузки OTA
      else 
      {
        isTouchOTA=false;
        clickTouchOTA=0;
      }
    }
    // Отмечаем завершение цикла задачи для сторожевого таймера
    flag[ftaskTouchscreen] = 1;
    TickType_t duration = xTaskGetTickCount() - start;
    //Serial.printf("Длительность taskTouchscreen(): %d ms\n", duration * portTICK_PERIOD_MS);
    // Если было введено число=fmessageReceived
    // if (inumber == ftaskMain) MimicMCUhangEvent("taskMain");   
    // Делаем задержку на чиску мусора 
    vTaskDelay(71);
  }
}

// *********************************************************** TouchPress.h ***
