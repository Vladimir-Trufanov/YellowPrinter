#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

// Создаем объект фона слайдера, который тянется на весь экран
//TFT_eSprite *backSlider;
TFT_eSprite backSlider = TFT_eSprite(&tft); // Sprite object 
// Создаем бегунок слайдера
//TFT_eSprite *Slider;
TFT_eSprite Slider = TFT_eSprite(&tft); // Sprite object 



void setup() 
{
  tft.init();
  tft.fillScreen(TFT_BLACK);

  // инициализация SPIFFS
  if (!SPIFFS.begin()) 
  {
    while (1) yield();
  }  

  // Create a sprite for the backSlider
  backSlider.setColorDepth(8);
  backSlider.createSprite(tft.width(), tft.height());
  backSlider.fillSprite(TFT_BLACK); // Note: Sprite is filled with black when created

  // Create a sprite for the Slider
  Slider.setColorDepth(8);
  Slider.createSprite(20, tft.height());
  Slider.fillSprite(TFT_YELLOW); // Note: Sprite is filled with black when created

}

int maxValue=100;
int minValue=5;

void loop() 
{
  static int currentValue=10;
  const int step=2; // скорость изменения
  // Обновляем позицию
  currentValue += step;
  if (currentValue>=maxValue) currentValue=maxValue;
  if (currentValue<=minValue) currentValue=minValue;

  // Перед отрисовкой фона и бегунка
  backSlider.fillSprite(TFT_BLACK);
  Slider.fillSprite(TFT_BLACK);

  // Рисуем обновленный слайдер
  //Slider.drawSprite(0,0);
  // Рисуем аолосу прогресса
  tft.drawRect(0,tft.height()-20,(float)currentValue/maxValue*(tft.width()-20),20,TFT_BLUE);
  delay(30);
}
