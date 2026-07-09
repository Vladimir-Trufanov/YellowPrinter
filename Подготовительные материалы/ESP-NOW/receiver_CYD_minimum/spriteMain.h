/** YellowPrinter-Esp32-Arduino                            *** spriteMain.h ***
 * 
 * Представить на экране дисплея поступившие строки и уложенные в спрайт
 * 
 * v1.0.0, 08.07.2026                                 Автор:      Труфанов В.Е.
 * Copyright © 2026 tve                               Дата создания: 12.04.2024
**/

#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>

char* IntToChar(uint16_t numbIn); 

class TSprite_Main 
{
  public:

  TSprite_Main(int iQueueSize=4); 
  void init(); 
  void sayLine(int16_t x, int16_t y, char* line); 
  void say(); 
  void clrScr(bool color); 

  private:

  TFT_eSPI tft = TFT_eSPI();
  TFT_eSprite stext3 = TFT_eSprite(&tft); 

  uint16_t i=0;
  char line[320];  // строка спрайта
  char fill[320];  // заполнитель строки


  /*
  // Определяем счетчик прерываний от таймера и общее их количество до
  // события отправки одного из четырех сообщений = чуть быстрее четверти секунды 
  unsigned int cntr = 0;                      // счетчик тиков таймера
  volatile unsigned int timerToggle = 21000;  // 62499 -> 1 секунда;
  volatile bool OneSecondFlag = false;        // истечение 1 сек для запуска трассировок
  volatile bool Motor1_Flag = false;          // истечение 1 четверти сек для отправки данных по мотору
  volatile bool Vcc2_Flag = false;            // истечение 2 четверти сек для отправки данных по напряжению батареи
  volatile bool Act3_Flag = false;            // истечение 3 четверти сек для подтверждения крайней команды
  volatile byte quatr = 0;                    // счетчик четвертей секунды
  */
};


  TSprite_Main::TSprite_Main(int iQueueSize) 
  {
    // Чистим заполнитель
    memset(fill,32,319); 
    fill[319]='\0';
    //
    int t=iQueueSize;
  };

  void TSprite_Main::clrScr(bool color) 
  {
    bool c=color;
  }

  void TSprite_Main::init() 
  {
    // Задаем среднюю глубину цвета и создаем спрайт
    // заданного размера в пикселах в оперативной памяти
    stext3.setColorDepth(8);
    stext3.createSprite(200, 110);
    // Заполняем буфер памяти, выделенный под спрайт, заданным цветом
    stext3.fillSprite(TFT_BLACK);
    // Отключаем перенос текста и по горизонтали и по вертикали 
    stext3.setTextWrap(false, false);
    // Определяем цвет текста с прозрачным фоном  
    stext3.setTextColor(TFT_WHITE,TFT_BLACK,true); 
    // Загружаем шрифт в память спрайта
    //stext3.loadFont("HuaweiSans16");     // загрузка в память шрифта
    stext3.loadFont("nasalization48");     
  }
  
  void TSprite_Main::sayLine(int16_t x, int16_t y, char* line) 
  {
    // Устанавливаем позицию курсора для вывода текста в заданной позиции спрайта
    stext3.setCursor(x,y);
    stext3.print(fill);
    stext3.setCursor(x,y);
    stext3.print(line); 
  }

  // Так как предполагается вывод всего 2 строк, то 
  // вначале выводим прежнее значение в нижней части
  void TSprite_Main::say() 
  {
    char chi[6]="Чи";
    char exclam[2]="!";

    memset(line,'\0',320); 
    strcat(line,chi); 
    strcat(line,IntToChar(i)); 
    strcat(line,exclam); 
    sayLine(10,55,line);

    // Выше складываем новое значение
    i++;
    memset(line,'\0',320); 
    strcat(line,chi); 
    strcat(line,IntToChar(i)); 
    strcat(line,exclam); 
    sayLine(10,10,line);
    
    // Выталкмваем спрайт на дисплей с заданной позиции
    stext3.pushSprite(10,60);
  }

  // ****************************************************************************
  // *            Преобразовать беззнаковое  целое в строку символов            *
  // ****************************************************************************
  char charNumby[6]; // char[5]+'\0'
  char* IntToChar(uint16_t numbIn) 
  {
    uint16_t numby=numbIn;
    memset(charNumby,'\0',6); 
    if (numby>65534) numby=0;
    String(numby).toCharArray(charNumby,6);
    return charNumby;
  }

  // *********************************************************** spriteMain.h ***
