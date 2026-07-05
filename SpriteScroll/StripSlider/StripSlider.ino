// StripSlider.ino

#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() 
{
  tft.init();
  tft.fillScreen(TFT_BLACK); // clear the screen
  // Set the TFT display rotation in landscape mode
  tft.setRotation(1);
}

void loop() 
{
  // Draw a rectangle with a red border
  tft.drawRect(10, 10, 50, 30, TFT_RED);
  // Draw a rounded rectangle with a blue border
  tft.drawRoundRect(10, 40, 60, 40, 10, TFT_BLUE);

  tft.fillRect(15, 0, 40, 200, TFT_WHITE);
  tft.drawSmoothRoundRect(10, 70, 10, 5, 50, 90, TFT_RED, TFT_BLACK);

  delay(1000);
}
