#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
  tft.init();
  tft.fillScreen(TFT_BLACK); // Clear the screen with black color
  // Set the TFT display rotation in landscape mode
  tft.setRotation(1);

}

void loop() {
  tft.drawFastVLine(50, 10, 100, TFT_GREEN); // Draw a vertical green line from (50, 10) to (50, 110)
  //tft.drawFastHLine(10, 20, 100, TFT_RED);   // Draw a horizontal red line from (10, 20) to (110, 20)
  delay(1000);
}
