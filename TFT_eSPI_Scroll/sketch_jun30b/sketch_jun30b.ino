// https://doc-tft-espi.readthedocs.io/tft_espi/methods/settextwrap/

#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI(); // Initialize the TFT display

void setup() {
  tft.begin(); // Initialize the TFT display
  tft.setRotation(1); // Set the display rotation to landscape
  tft.setTextSize(2); // Set the text size to 2
}

void loop() {
  String weatherDescription1 = 
    "1111 Partly cloudy with a high chance of scattered thunderstorms in the afternoon. Winds will be moderate with gusts up to 30 km/h.";
  String weatherDescription2 = 
    "2222 Partly cloudy with a high chance of scattered thunderstorms in the afternoon. Winds will be moderate with gusts up to 30 km/h.";
  String weatherDescription3 = 
    "3333 Partly cloudy with a high chance of scattered thunderstorms in the afternoon. Winds will be moderate with gusts up to 30 km/h.";

  tft.fillScreen(TFT_BLACK); // Clear the screen
  tft.setTextColor(TFT_WHITE); // Set the text color to white

  // Set the text wrap mode to true for horizontal wrapping and false for vertical wrapping
  tft.setTextWrap(true, false);

  // Print the weather description
  tft.setCursor(10, 20); // Set the cursor to (10, 20)
  tft.print(weatherDescription1);
  tft.print(weatherDescription2);
  tft.print(weatherDescription3);

  delay(10000); // Wait for 10 seconds
}
