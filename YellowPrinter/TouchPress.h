/** YellowPrinter-Esp32-Arduino                            *** TouchPress.h ***
 * 
 * Обнаружить нажатие на сенсорной панели и зафиксировать данные нажатия
 * 
 * v1.0.0, 12.08.2026                                 Автор:      Труфанов В.Е.
 * Copyright © 2026 tve                               Дата создания: 12.08.2026
**/

#pragma once

#include <Arduino.h>
#include "inimem.h"


// st - префикс спрайта spriteTouch - данных по нажатию на сенсорной панели 
#define stLINESIZE    80    // размер буфера одного сообщения
#define stWIDTH      304    // ширина спрайта сообщений 
#define stHEIGHT      16    // высота спрайта
#define stTOP        212    // позиция по вертикали размещения спрайта на экране 
#define stLEFT        16    // позиция по горизонтали

/*
#define smLINEHEIGHT  16    // высота строки в спрайте (px)
#define smLINEOFFSET   2    // смещение текста от левого края спрайта (px)
#define smMAXLINE     13    // число строк в спрайте
*/

TFT_eSprite spriteTouch = TFT_eSprite(&tft);

class TSprite_Touch 
{
  public:

  TSprite_Touch(); 
  void View();

  private:

  char line[stLINESIZE];  // буфер cообщения
  uint16_t touchCalc;     // счетчик нажатий на сенсорную панель


};

TSprite_Touch::TSprite_Touch() 
{
  // Чистим заполнитель
  //memset(fill,32,smLINESIZE-1); 
  //fill[smLINESIZE-1]='\0';
  touchCalc=0;
};

void TSprite_Touch::View()
{
  // Изменяем счетчик
  touchCalc++;

  //
  spriteTouch.setColorDepth(8);
  spriteTouch.createSprite(stWIDTH, stHEIGHT);
  if (spriteTouch.created())
  {
    getheap("Создан spriteTouch");
    // Заполняем буфер памяти, выделенный под спрайт, заданным цветом
    spriteTouch.fillSprite(TFT_BLACK);
    // Отключаем перенос текста и по горизонтали и по вертикали 
    spriteTouch.setTextWrap(false, false);
    // Определяем цвет текста с прозрачным фоном  
    spriteTouch.setTextColor(TFT_WHITE,TFT_BLACK,true); 
    // Загружаем шрифт в память спрайта
    spriteTouch.loadFont("HuaweiSans16");   
    // Чистим строку
    //memset(line,32,stLINESIZE-1); 
    //line[stLINESIZE-1]='\0';

    memset(line,'\0',stLINESIZE); 
    strcat(line,"touchCalc = "); 
    strcat(line,IntToChar(touchCalc)); 

    Serial.println(line);
    getheap("Сделана строка spriteTouch");

    spriteTouch.setCursor(0,0);
    spriteTouch.print(line);
    spriteTouch.pushSprite(stLEFT,stTOP);

    spriteTouch.unloadFont();             // выгрузка шрифта из памяти
    spriteTouch.deleteSprite();
  }
  else
  {
    Serial.println("НЕ ПОЛУЧИЛОСЬ создать spriteTouch!");
  }
}

// Счетчик нажатий на сенсорную панель
//uint16_t touchCalc=0;
// Объект для работы с сенсорной панелью
TSprite_Touch ypsTouch;

void taskTouchscreen (void *pvParameters) 
{
  char mess[smLINESIZE];      // буфер входного сообщения
  while (1) 
  {
    // Фиксируем начало цикла задачи
    //touchCalc++;
    TickType_t start = xTaskGetTickCount();

    //memset(mess,'\0',smLINESIZE); 
    //strcat(mess,"touchCalc = "); 
    //strcat(mess,IntToChar(touchCalc)); 
    
    //Serial.println(mess);
    ypsTouch.View();


    //tft.setCursor(0, 212);
    //tft.print("Начало 222");
    
  


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
    // Отмечаем завершение цикла задачи для сторожевого таймера
    flag[ftaskTouchscreen] = 1;
    TickType_t duration = xTaskGetTickCount() - start;
    //Serial.printf("Длительность taskTouchscreen(): %d ms\n", duration * portTICK_PERIOD_MS);
    // Если было введено число=fmessageReceived
    // if (inumber == ftaskMain) MimicMCUhangEvent("taskMain");   
    // Делаем задержку на чиску мусора 
    vTaskDelay(1064);
  }
}



// *********************************************************** TouchPress.h ***
