// get_mac_address.ino
// Wolfgang Ewald [https://wolles-elektronikkiste.de/en/esp-now]

#define STATYPE 8266
//#define STATYPE 32

#if (STATYPE == 8266)
  #include <ESP8266WiFi.h> // for ESP8266 boards
#else
  #include "WiFi.h"
#endif

void setup()
{
  Serial.begin(115200);
  delay(1000);   // uncomment if your serial monitor is empty
  Serial.println ("\n---------------------");
  WiFi.mode(WIFI_STA); 
  

  #if (STATYPE == 8266)
    delay(1000); // uncomment for ESP8266
  #else
    while (!(WiFi.STA.started())) 
    { // comment the while loop for ESP8266
      delay(10);
    }
  #endif

  Serial.print("MAC-Address: ");
  String mac = WiFi.macAddress();
  Serial.println(mac);
    
  Serial.print("Formated: ");
  Serial.print("{");
  int index = 0;
  for(int i=0; i<6; i++)
  {
    Serial.print("0x");
    Serial.print(mac.substring(index, index+2));
    if(i<5)
    {
      Serial.print(", ");
    }
    index += 3;
  }
  Serial.println("}");
}
 
void loop(){}