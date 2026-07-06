// GetMac.ino - узнать mac-адрес контроллера

/*
 AI Thinker ESP32-CAM 6  - MAC Address: F8:B3:B7:A7:E1:18
 ESP32-2432S028 (CYD) 7  - MAC Address: B8:D6:1A:5C:F9:54
*/

#ifdef ESP32
  #include <WiFi.h>
#else 
 #include <ESP8266WiFi.h>
#endif

void setup() 
{ 
  Serial.begin(115200);
  Serial.println(); 
  // Initialize the Wi-Fi module
  WiFi.mode(WIFI_STA);
  while (!WiFi.STA.started()) 
  {
    delay(100);
  }
  Serial.println("MAC Address: " + WiFi.macAddress());
}

void loop() 
{
}
